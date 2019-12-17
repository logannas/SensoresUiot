#ifndef UHttp_esp8266_H
#define UHttp_esp8266_H

#include <ESP8266HTTPClient.h>
#include "BaseProtocol_esp8266.h"
#include <WiFiManager.h>
#include <ESP8266WiFi.h>
#include <ArduinoOTA.h>

class UHttp_esp8266 : public BaseProtocol_esp8266{
    public:
        UHttp_esp8266();
        UHttp_esp8266(String);
        void OTA_config(String);
        void OTA_config(String, bool);
        void init(String);
		void handle();
        void set_server(String);
        bool register_device();
        bool register_service(int);
        bool register_data(int, char*, int);

    private:
        // PubSubClient mqtt_client;
        String server;
        HTTPClient http;
        int mqtt_port = 1883;
};





#endif
