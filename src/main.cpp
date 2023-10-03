#include <Arduino.h>
#include <OTA/OTA.h>
#include <Wifi/Wifi.h>
#include <DHT/Printer_DHT.h>
#include <PDisplay/PDisplay.h>
#include <SensorAHT/SensorAHT.h>
#include <FileSystem/FileSystem.h>
#include <ArduinoJson.h>
#include <Server/Server.h>
#include <Time/Time.h>

const char* ssid = "Ant-LAN and the WiFi";
const char* password = "Random221105";

void getData(JsonDocument& doc){
    doc["humidity"] = sensorAHT.getHumidity();
    doc["temperature"] = sensorAHT.getTemperatureC();
    doc["timeStamp"] = hshTime.getEpoch();
}

void setup() {
    Serial.begin(115200);
    db.begin();
    pDisplay.init();
    if( wifi.init(ssid,password) ){
        Serial.printf("Wifi Connected to %s IP is: %s\n", ssid, WiFi.localIP().toString().c_str());
    }
    hshTime.begin(true);
    hshServer.begin();
    ota.init();
    sensorAHT.init();

    hshTime.onSecChange([](int prev, int now){
        pDisplay.print(3,0,
            "%d %02d/%02d",
            hshTime.getYear(),
            hshTime.getMonth(),
            hshTime.getDay()
        );
        
        pDisplay.print(4,2,
            "%02d:%02d:%02d",
            hshTime.getHour(),
            hshTime.getMin(),
            hshTime.getSec()
        );

        pDisplay.print(0,5,
            "%.2f%% | %.2fC",
            sensorAHT.getHumidity(),
            sensorAHT.getTemperatureC()
        );
        pDisplay.printDegreeSysmbol();
    });

    hshTime.onHourChange([](int prev, int now){
        DynamicJsonDocument doc(1024);
        getData(doc);
        db.append("/data.json", doc);
        hshServer.sendEvent("humidAndTime", doc);
    });
}

void loop() {
    ota.loop();
    sensorAHT.loop();
    hshTime.loop();
}