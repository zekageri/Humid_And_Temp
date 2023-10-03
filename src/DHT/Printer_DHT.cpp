#include "Printer_DHT.h"
#include <Adafruit_Sensor.h>
#include <DHT.h>

DHT dht(DHT_SENSOR_PIN, DHT_SENSOR_TYPE);

void Printer_DHT::init(){
    dht.begin();
}

void Printer_DHT::onRefresh(onNewDataCB cb){
    onNewDataCbs.push_back(cb);
}

void Printer_DHT::emitEvents(){
    for( onNewDataCB cb : onNewDataCbs ){
        cb();
    }
}

void Printer_DHT::loop(){
    if( millis() - lastRead_MS >= SENSOR_READ_INTERVAL_MS ){
        lastRead_MS = millis();
        humi = dht.readHumidity();
        tempC = dht.readTemperature();
        tempF = dht.readTemperature(true);
        emitEvents();
        
        if( debug ){
            if (isnan(tempC) || isnan(tempF) || isnan(humi)) {
                Serial.println("Failed to read from DHT sensor!");
            } else {
                Serial.printf("\nHumidity: ");
                Serial.print(humi);
                Serial.print("%");

                Serial.print("  |  ");

                Serial.print("Temperature: ");
                Serial.print(tempC);
                Serial.print("°C  ~  ");
                Serial.print(tempF);
                Serial.println("°F");
            }
        }
    }
}

float Printer_DHT::getHumidity(){
    return humi;
}

float Printer_DHT::getTemperatureC(){
    return tempC;
}

float Printer_DHT::getTemperatureF(){
    return tempF;
}