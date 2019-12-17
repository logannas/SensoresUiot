#include "DHT.h"
#include "UHttp_esp8266.h"

UHttp_esp8266 device;
String device_name = "Estação_Teste";

#define DHTPIN 0     // onde o dht11 está conectado --- Wemos D1 R1 - D9 - define 2 /// Esp01 - GPIO 0 - define 0
#define DHTTYPE DHT11 //definindo dht11  

DHT dht(DHTPIN, DHTTYPE); //Incluindo o DHT11

int service1, service2,service3;   //variáveis para criar os serviços

int sensor = A0;    //Váriaveis para o sensor ldr
int valorldr = 0;  
const int sleepSeconds = 1;

void setup() {
  Serial.begin(9600);   //Iniciando o serial begin

  device.init(device_name);      //Inicializando o device
  device.set_server("http://172.16.9.231:5000");     //Conectando ao servidor do IOT
  service1 = device.create_service(1,"Luminosidade","%",true,"Luminosidade estimada");     //Especificando os serviços
  service2 = device.create_service(2,"Temperatura","ºC",true,"Temperatura estimada");
  service3 = device.create_service(3,"Humidade","%",true,"Humidade estimada");

  dht.begin();      //Ligando o dht
  //pinMode(D2, WAKEUP_PULLUP);
  
    Serial.println("I'm awake.");
       device.handle();
    if(WiFi.status()==WL_CONNECTED /*&& Serial.available()*/){
      float h = dht.readHumidity();
      // Temperatura em Celsius
      float t = dht.readTemperature();
      valorldr = analogRead(sensor);
      valorldr = map(valorldr, 1023, 0, 0, 100);
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
    
  
      
    Serial.println("Going into deep sleep for 1m");
    ESP.deepSleep(1 * 60000000);
      //delay(2000);
  }

}

void loop(){
 
}
