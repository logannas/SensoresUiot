/*Estação Meteriológica - Projeto Laboratório UIOT - UNB 
 * Desenvolvido por Anna Carolina F. Rosa e Daniel Prado
 * Usado Esp 01 e DHT11
 * DHT11 conectado ao GPIO2 da Esp01 
 * Para o low power foi usado o GPIO16 ligado ao Reset
 * Dados enviados por IP
 */


#include "DHT.h"
#include "UHttp_esp8266.h"

UHttp_esp8266 device;
String device_name = "Estação_Teste";

  #define DHTPIN 2     // onde o dht11 está conectado --- Wemos D1 R1 - D9 - define 2 /// Esp01 - GPIO 2 - define 2
  #define DHTTYPE DHT11 //definindo dht11  

  DHT dht(DHTPIN, DHTTYPE); //Incluindo o DHT11

int service1, service2;   //variáveis para criar os serviços

//const int sleepSeconds = 1;

const char* ssid     = "UIOT";         // The SSID (nome) do Wifi que está tentando se conectar
const char* password = "A12818UIOT";     // Senha do Wifi

void setup() {
 Serial.begin(9600);   //Iniciando o serial begin
 WiFi.begin(ssid, password);    //Iniciando o wifi com ssid e senha
  
  device.init(device_name);      //Inicializando o device
  device.set_server("http://172.16.9.231:5000");     //Conectando ao servidor do IOT
  service1 = device.create_service(1,"Temperatura","ºC",true,"Temperatura estimada");   //Especificando serviço de temperatura
  service2 = device.create_service(2,"Umidade","%",true,"Umidade estimada");          //Especificando serviço de umidade

  dht.begin();      //Ligando o dht
  
  device.handle();
  
    if(WiFi.status()==WL_CONNECTED /*&& Serial.available()*/){    //Checando a conexão Wifi
      //Humidade em porcentagem
      float h = dht.readHumidity();
      // Temperatura em Celsius
      float t = dht.readTemperature();

      device.send_data(service1, (char*)String(t).c_str(), 0);      //Enviando os dados de temperatura
      device.send_data(service2, (char*)String(h).c_str() ,0);      //Enviando os dados de humidade
       
   
      ESP.deepSleep(1 * 60000000);                                  //Esperando 1 minuto para reenviar os dados 

    }
  
}

void loop(){
  
}
