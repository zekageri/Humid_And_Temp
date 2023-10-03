#include "Wifi.h"
#include <WiFi.h>

boolean Wifi::init(const char* ssid, const char* password){
    if( !ssid || !password ){
        return false;
    }
    WiFi.mode(WIFI_STA);
    WiFi.begin(ssid, password);
    uint8_t status = WiFi.waitForConnectResult(WIFI_TIMEOUT);
    return status == WL_CONNECTED;
}