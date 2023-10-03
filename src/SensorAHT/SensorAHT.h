#pragma once

#include <Arduino.h>
#include <vector>

#define DHT_SENSOR_PIN 14
#define DHT_SENSOR_TYPE DHT11
#define SENSOR_READ_INTERVAL_MS 500

using onNewDataCB = std::function<void()>;

class SensorAHT{
    public:
        void init();
        void loop();

        float getHumidity();
        float getTemperatureC();

        void onRefresh(onNewDataCB cb);
    private:
        // Create a timeGetFunc vector
        std::vector<onNewDataCB> onNewDataCbs;

        void emitEvents();
        void printStatus();
        
        boolean debug = true;
        float humi,tempC,tempF;
        long lastRead_MS = 0;
};

inline SensorAHT sensorAHT;