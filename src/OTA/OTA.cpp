#include "OTA.h"

void OTA::init() {
    ArduinoOTA.setPort(3232);
    ArduinoOTA.setHostname("Humid_And_Temp");
    ArduinoOTA.onStart([]() {
        Serial.println("\n[OTA] - Starting.");
    }).onEnd([]() {
        Serial.println("\nEnd");
    }).onProgress([](unsigned int progress, unsigned int total) {
        Serial.printf("[OTA] - Progress: %u%%\r", (progress / (total / 100)));
    }).onError([](ota_error_t error) {
        Serial.printf("[OTA] - Error[%u]: ", error);
        if (error == OTA_AUTH_ERROR)
            Serial.println("Auth Failed");
        else if (error == OTA_BEGIN_ERROR)
            Serial.println("Begin Failed");
        else if (error == OTA_CONNECT_ERROR)
            Serial.println("Connect Failed");
        else if (error == OTA_RECEIVE_ERROR)
            Serial.println("Receive Failed");
        else if (error == OTA_END_ERROR)
            Serial.println("End Failed");
    });

    ArduinoOTA.begin();
}

void OTA::loop(){
    ArduinoOTA.handle();
}