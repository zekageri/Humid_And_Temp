#include "Server.h"

#include <FileSystem/FileSystem.h>
#include <Time/Time.h>

AsyncWebServer server(SERVER_PORT);
AsyncEventSource events(EVENT_SOURCE);

void HsH_Server::begin() {
    DefaultHeaders::Instance().addHeader("Server", "ESP_Humid&Temp");
    server.serveStatic("/", db.userFS, "/").setCacheControl(ONE_HOUR_CACHE_TIME);

    setupEvents();

    server.on("/", HTTP_GET, [this](AsyncWebServerRequest* request) {
        AsyncWebServerResponse* response = request->beginResponse(
            db.userFS,
            INDEX_PATH,
            "text/html"
        );
        request->send(response);
    });

    server.on("/db/download", HTTP_GET, [](AsyncWebServerRequest *request){
        if( !request->authenticate("admin","admin") ){
            request->requestAuthentication();
            return;
        }
        if( !request->hasParam("path") ){
            request->send(400);
            return;
        }
        String path = request->getParam("path")->value();
        request->send(db.userFS, path, "application/octet-stream");
    });
    
    server.on("/db/delete", HTTP_GET, [](AsyncWebServerRequest *request){
        if( !request->authenticate("admin","admin") ){
            request->requestAuthentication();
            return;
        }
        if( !request->hasParam("path") ){
            request->send(400);
            return;
        }
        String path = request->getParam("path")->value();
        boolean resp = db.remove(path.c_str());
        request->send(resp?200:500, "text/plain", resp?"Success":"Failed");
    });

    server.on("/restart", HTTP_GET, [](AsyncWebServerRequest* request) {
        request->send(200, "text/plain", "Restarting...");
        ESP.restart();
    });

    server.addHandler(&events);
    server.onNotFound([](AsyncWebServerRequest* request) {
        request->send(404, "text/plain", "Not Found!");
    });
    server.begin();
}

void HsH_Server::loop() {
    // ws.cleanupClients();
}

void HsH_Server::setupEvents() {
    events.onConnect([this](AsyncEventSourceClient* client) {
        if (debug) {
            Serial.printf("[Events] - Client connected!\n");
        }
        if (client->lastId()) {
            Serial.printf("[Events] - Client reconnected! Last message ID that it got is: %u\n", client->lastId());
        }
        client->send("Hello user!", NULL, millis(), 1000);
    });
}

boolean HsH_Server::hasEventClient() {
    return events.count() > 0;
}

void HsH_Server::sendEvent(const char* type, const char* message, ...) {
    if (!hasEventClient()) {
        return;
    }
    size_t size = strlen(message) + 1;
    char buffer[size];
    va_list args;
    va_start(args, message);
    vsprintf(buffer, message, args);
    va_end(args);
    events.send(buffer, type, hshTime.getEpoch());
}

void HsH_Server::sendEvent(const char* type, JsonDocument& doc) {
    if (!hasEventClient()) {
        return;
    }
    size_t size = doc.capacity() + 1;
    char buffer[size];
    serializeJson(doc, buffer, size);
    events.send(buffer, type, hshTime.getEpoch());
}