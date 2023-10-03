#include "SensorAHT.h"

#include <AHTxx.h>

AHTxx aht10(AHTXX_ADDRESS_X38, AHT1x_SENSOR);

void SensorAHT::init() {
    if (aht10.begin()) {
        Serial.printf("AHT10 Initialised!");
    }
    printStatus();
}

float SensorAHT::getHumidity() {
    return humi;
}

float SensorAHT::getTemperatureC() {
    return tempC;
}

void SensorAHT::onRefresh(onNewDataCB cb) {
    onNewDataCbs.push_back(cb);
}

void SensorAHT::emitEvents() {
    for (onNewDataCB cb : onNewDataCbs) {
        cb();
    }
}

void SensorAHT::printStatus() {
    switch (aht10.getStatus()) {
        case AHTXX_NO_ERROR:
            Serial.println(F("no error"));
            break;

        case AHTXX_BUSY_ERROR:
            Serial.println(F("sensor busy, increase polling time"));
            break;

        case AHTXX_ACK_ERROR:
            Serial.println(F("sensor didn't return ACK, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
            break;

        case AHTXX_DATA_ERROR:
            Serial.println(F("received data smaller than expected, not connected, broken, long wires (reduce speed), bus locked by slave (increase stretch limit)"));
            break;

        case AHTXX_CRC8_ERROR:
            Serial.println(F("computed CRC8 not match received CRC8, this feature supported only by AHT2x sensors"));
            break;

        default:
            Serial.println(F("unknown status"));
            break;
    }
}

void SensorAHT::loop() {
    if (millis() - lastRead_MS >= SENSOR_READ_INTERVAL_MS) {
        lastRead_MS = millis();
        humi = aht10.readHumidity();
        tempC = aht10.readTemperature();
        emitEvents();
    }
}