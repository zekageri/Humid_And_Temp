#pragma once

#include <Arduino.h>
#include <ESPAsyncWebserver.h>
#include <ArduinoJson.h>

#define SERVER_PORT 80
#define EVENT_SOURCE "/humidAndTime"

#define MAX_CACHE_CONTROL_TIME "max-age=31536000"
#define ONE_DAY_CACHE_TIME "max-age=86400"
#define ONE_HOUR_CACHE_TIME "max-age=3600"
#define SEVE_DAY_CACHE_TIME "max-age=172800"

#define INDEX_PATH "/index.html"

class HsH_Server{
    public:
        void begin();
        void loop();
        void sendEvent(const char* type, const char* message, ...);
        void sendEvent(const char* type, JsonDocument& doc);
        boolean hasEventClient();
        boolean getCookie(String name, String cookie, String &result);
        boolean getCookie(const char* name, const char* cookie, const char*& result);
        void setCookie(AsyncWebServerResponse* response, const char* name, const char* value, int maxAgeInSeconds);
        String getMimeType(String path);
    private:
        boolean debug = true;
        void setupEvents();
};

inline HsH_Server hshServer;