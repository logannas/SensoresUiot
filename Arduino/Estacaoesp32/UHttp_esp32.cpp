#include "UHttp_esp32.h"

UHttp_esp32::UHttp_esp32(){}

void UHttp_esp32::init(String device_name){
  delay(1000);
  this->device_identificator();
  this->name = device_name;
  WiFiManager wifimanager;
  wifimanager.autoConnect(device_name.c_str());
  Serial.println("Connection Successfull");
  this->OTA_config(device_name, false);
	Serial.println("Fim do init");
	//while (Serial.read()==-1);
}


void UHttp_esp32::handle(){
	ArduinoOTA.handle();
}

// void UHttp_esp32::init(String device_name, bool force_erase_all){
//   delay(1000);
//   this->device_identificator();
//   this->name = device_name;
//
//   this->OTA_config(device_name, force_erase_all);
//   WiFiManager wifimanager;
//   wifimanager.autoConnect(device_name.c_str());
//   Serial.println("Connection Successfull");
// }

void UHttp_esp32::OTA_config(String ota_name, bool force_erase_all){
  ArduinoOTA
    .onStart([]() {
      String type;
      if (ArduinoOTA.getCommand() == U_FLASH)
        type = "sketch";
      else // U_SPIFFS
        type = "filesystem";

      // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
      Serial.println("Start updating " + type);
    })
    .onEnd([]() {
      Serial.println("\nEnd");
    })
    .onProgress([](unsigned int progress, unsigned int total) {
      Serial.printf("Progress: %u%%\r", (progress / (total / 100)));
    })
    .onError([](ota_error_t error) {
      Serial.printf("Error[%u]: ", error);
      if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
      else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
      else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
      else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
      else if (error == OTA_END_ERROR) Serial.println("End Failed");
    });
  ArduinoOTA.setHostname(ota_name.c_str());
  ArduinoOTA.setPassword((ota_name + "_password").c_str());
  ArduinoOTA.begin();
}


void UHttp_esp32::set_server(String server){
  //Serial.println("set_server");
  this->server = server;
}

bool UHttp_esp32::register_device(){
  char *data = NULL;
  data = this->make_client_data(data);
  Serial.print("Sending:\"");
  Serial.print(data);
  Serial.println("\"");
  this->http.begin(this->server + "/client");
  this->http.addHeader("Content-Type", "application/json");
  int code = this->http.POST(data);
  String payload = this->http.getString();
  Serial.println(code);
  Serial.print("Recieved:\"");
  Serial.print(payload);
  Serial.println("\"");
  free(data);
  this->http.end();
  return (code==200);
}
bool UHttp_esp32::register_service(int s){
  char *data = NULL;
  // data = this->make_service_data(s, data);
  data = this->make_service_data(this->service[s], data);
  Serial.println("Sending:\"");
  Serial.println(data);
  Serial.println("\"");
  this->http.begin(this->server + "/service");
  this->http.addHeader("Content-Type", "application/json");
  int code = this->http.POST(data);
  String payload = this->http.getString();
  Serial.println(code);
  Serial.print("Recieved:\"");
  Serial.print(payload);
  Serial.println("\"");
  free(data);
  this->http.end();
  return (code==200);
}
bool UHttp_esp32::register_data(int s, char* value, int sensitive){
  char *data = NULL;
  // data = this->make_raw_data(s, value, sensitive, data);
  data = this->make_raw_data(this->service[s], value, sensitive, data);
  Serial.print("Sending:\"");
  Serial.print(data);
  Serial.println("\"");
  this->http.begin(this->server + "/data");
  this->http.addHeader("Content-Type", "application/json");
  int code = this->http.POST(data);
  String payload = this->http.getString();
  Serial.println(code);
  Serial.print("Recieved:\"");
  Serial.print(payload);
  Serial.println("\"");
  free(data);
  this->http.end();
  return (code==200);
}
