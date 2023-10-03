#pragma once

#include <Arduino.h>
#include <ArduinoOTA.h>

class OTA{
    public:
        void init();
        void loop();
    private:
};

inline OTA ota;