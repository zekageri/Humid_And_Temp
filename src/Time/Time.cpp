#include <Time/time.h>
#include <ArduinoJson.h>

void Time::begin(boolean hasNetwork){
    syncNTP();
}

void Time::syncNTP(){
    if( debug ){
        Serial.printf("[Time] - Syncronizing time from ntp...\n");
    }
    int gmtOffset_sec = 60 * 60 * 60;
    int daylightOffset_sec = 60 * 60 * 60;
    configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);
    setTimeZone( "GMT-02:00" );
    if(!getLocalTime(&timeinfo)){
        if( debug ){
            Serial.printf("[Time] - Failed to obtain NTP time\n");
        }
        return;
    }
    emitTimeGet();
    if( debug ){
        Serial.printf(
            "[Time] - Got NTP time: %d-%d-%d %d:%d:%d\n",
            timeinfo.tm_year+1900,
            timeinfo.tm_mon + 1,
            timeinfo.tm_mday,
            timeinfo.tm_hour,
            timeinfo.tm_min,
            timeinfo.tm_sec
        );
    }
}


void Time::setTimeZone(const char* GMT_String){
    setenv("TZ", GMT_String, 1);
    tzset();
}


boolean Time::setTimeFromStringObject(const char* timeString) {
    DynamicJsonDocument doc(1024);
    DeserializationError error = deserializeJson(doc, timeString);
    if (error) { return false; }
    setTime(doc["year"],doc["month"],doc["dayOfMonth"],doc["hour"],doc["minute"],doc["second"]);
    return true;
}

void Time::setTime(int year,int month,int day,int hour,int minute,int second){
    timeinfo.tm_year = year - 1900;
    timeinfo.tm_mon = month - 1;
    timeinfo.tm_mday = day;
    timeinfo.tm_hour = hour;
    timeinfo.tm_min = minute;
    timeinfo.tm_sec = second;

    time_t t = mktime(&timeinfo);
    struct timeval now = {.tv_sec = t};
    settimeofday(&now, nullptr);
}

void Time::handleUptime() {
    if (millis() - lastUptimeCheck >= 1000) {
        lastUptimeCheck = millis();
        if (millis() >= 3000000000) {
            HighMillis = 1;
        }
        if (millis() <= 100000 && HighMillis == 1) {
            Rollover++;
            HighMillis = 0;
        }
        long secsUp = millis() / 1000;
        Up_Second = secsUp % 60;
        Up_Minute = (secsUp / 60) % 60;
        Up_Hour = (secsUp / (60 * 60)) % 24;
        Up_Day = (Rollover * 50) + (secsUp / (60 * 60 * 24));
        sprintf(uptimeString, "%dD %02d:%02d:%02d", Up_Day, Up_Hour, Up_Minute, Up_Second);
        emitUptimeChange(Up_Day, Up_Hour, Up_Minute, Up_Second);
    }
}

uint32_t Time::getEpoch(boolean inMilliseconds){
    time_t epochSeconds = mktime(&timeinfo);
    if (inMilliseconds) {
        return epochSeconds * 1000;
    }
    return epochSeconds;
}

void Time::updateTime(){
    getLocalTime(&timeinfo);
}

void Time::loop(){
    handleEvents();
    handleUptime();
    updateTime();
}