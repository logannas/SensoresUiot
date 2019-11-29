#include "DHT.h"
#include "UHttp_esp8266.h"

UHttp_esp8266 device;
String device_name = "Estação_Teste";

#define DHTPIN 2     // onde o dht11 está conectado
#define DHTTYPE DHT11 //definindo dht11  

DHT dht(DHTPIN, DHTTYPE); //Incluindo o DHT11

int service1, service2,service3;   //variáveis para criar os serviços

int sensor = A0;    //Váriaveis para o sensor ldr
int valorldr = 0;  

void setup() {
  Serial.begin(9600);   //Iniciando o serial begin

  device.init();      //Inicializando o device
  device.set_server("http://172.16.9.69:8000");     //Conectando ao servidor do IOT
  service1 = device.create_service(1,"Luminosidade","Ohms",true,"Luminosidade estimada");     //Especificando os serviços
  service2 = device.create_service(2,"Temperatura","ºC",true,"Temperatura estimada");
  service3 = device.create_service(3,"Humidade","%",true,"Humidade estimada");
  
  dht.begin();      //Ligando o dht
}

void loop(){
  //device.handle();

   if(WiFi.status()==WL_CONNECTED && Serial.available()){
    float h = dht.readHumidity();
   // Temperatura em Celsius
    float t = dht.readTemperature();
    valorldr = analogRead(sensor);

    //Imprimir no serial motor
    Serial.print("Valor LDR:");
    Serial.println(valorldr);
    Serial.print("Humidade: ");
    Serial.println(h);
    Serial.print("Temperatura: ");
    Serial.println(t);
    Serial.println("  ");

    device.send_data(service1, (char*)String(valorldr).c_str(),0);
    device.send_data(service2, (char*)String(t).c_str(), 0);
    device.send_data(service3, (char*)String(h).c_str() ,0);
  
  
  
  delay(2000);
  }
}
