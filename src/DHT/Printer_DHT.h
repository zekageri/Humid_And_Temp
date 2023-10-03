#pragma once

#include <Arduino.h>
#include <vector>

#define DHT_SENSOR_PIN 14
#define DHT_SENSOR_TYPE DHT11
#define SENSOR_READ_INTERVAL_MS 2000

using onNewDataCB = std::function<void()>;

class Printer_DHT{
    public:
        void init();
        void loop();

        float getHumidity();
        float getTemperatureC();
        float getTemperatureF();

        void onRefresh(onNewDataCB cb);
    private:
        // Create a timeGetFunc vector
        std::vector<onNewDataCB> onNewDataCbs;

        void emitEvents();
        
        boolean debug = true;
        float humi,tempC,tempF;
        long lastRead_MS = 0;
};

inline Printer_DHT sensor;