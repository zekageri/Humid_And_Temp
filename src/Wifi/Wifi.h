#pragma once

#include <Arduino.h>

#define WIFI_TIMEOUT 15000
class Wifi{
    public:
        boolean init(const char* ssid, const char* password);
    private:
};

inline Wifi wifi;