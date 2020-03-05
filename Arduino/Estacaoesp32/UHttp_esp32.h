#ifndef UHttp_esp32_H
#define UHttp_esp32_H

#include <HTTPClient.h>
#include "BaseProtocol_esp32.h"
#include <WiFiManager.h>
#include <WiFi.h>
#include <ArduinoOTA.h>

class UHttp_esp32 : public BaseProtocol_esp32{
    public:
        UHttp_esp32();
        UHttp_esp32(String);
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
