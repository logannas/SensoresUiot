  #include "UHttp_esp32.h"
  #include "DHT.h"
  #include <Wire.h>
  #include <SPI.h>
  #include <Adafruit_BMP085.h>
  #include "WiFi.h"

  #define DHTPIN 23     // onde o dht11 está conectado --- Wemos D1 R1 - D9 - define 2 /// Esp01 - GPIO 2 - define 2 //Esp32 gpio 23
  #define DHTTYPE DHT11 //definindo dht11  
  #define LDR 4 
  #define uS_TO_S_FACTOR 1000000  // Convertendo de microssegundos para segundos 
  #define TIME_TO_SLEEP  60        //Quantos segundos a esp vai dormir 
  #define MQ 34
  #define BT 33


UHttp_esp32 device;
String device_name = "EM";

int service0, service1, service2, service3, service4, service5;

const char* ssid     = "UIOT";         // The SSID (nome) do Wifi que está tentando se conectar
const char* password = "A12818UIOT";     // Senha do Wifi

DHT dht(DHTPIN, DHTTYPE); //Incluindo o DHT11
Adafruit_BMP085 bmp;      //Incluindo BMP


void setup() {
  Serial.begin(9600);                                  //Serial connection
  pinMode(4, INPUT);
  dht.begin();      //Inicinado dht
  bmp.begin();     //Iniciando bmp
  WiFi.begin(ssid, password);    //Iniciando o wifi com ssid e senha

  device.init(device_name);
  device.set_server("http://172.16.9.231:5000");
  service0 = device.create_service(0, "Humidade", "%", true, "Humidade local");
  service1 = device.create_service(1, "Luminosidade", "%", true, "Luminosidade local");
  service2 = device.create_service(2, "Temperatura", "%", true, "Temperatura local");
  service3 = device.create_service(3, "Pressão", "pa", true, "Pressão local");
  service4 = device.create_service(4, "CO2", "ppm", true, "CO2 local");
  service5 = device.create_service(5, "Bateria", "%", true, "Nível de bateria");


device.handle();
  if(WiFi.status()== WL_CONNECTED){   //Check WiFi connection status
    float h = dht.readHumidity();     //Humidade
    float b = analogRead(BT);   //Nível de Bateria
    b = map(analogRead(33), 0.0f, 4095.0f, 0, 100);
    int l = analogRead(LDR);           //LDR
    l = map(l, 1023, 0, 0, 100);
    if(l<0){
      l = 0;
    }
    float t2 = bmp.readTemperature();    //Temperatura bmp
    float p = bmp.readPressure();         //Pressão
    float a = bmp.readAltitude();         //Altitude
    float g = analogRead(MQ);              //CO2
    
    Serial.println("Sending message to Gateway...");

    
    device.send_data(service0, (char*)String(h).c_str(), 0);
    device.send_data(service1, (char*)String(l).c_str(), 0);
    device.send_data(service2, (char*)String(t2).c_str(), 0);
    device.send_data(service3, (char*)String(p).c_str(), 0);
    device.send_data(service4, (char*)String(g).c_str(), 0);
    device.send_data(service5, (char*)String(b).c_str(), 0);
    
     Serial.println("Esp indo dormir");

     Serial.println("");
      
  }

    //Low power -- timer
       esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
       esp_deep_sleep_start();

}

void loop() {
  
}
