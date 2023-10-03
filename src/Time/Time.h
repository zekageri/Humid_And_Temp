#pragma once
#include <Arduino.h>

#define TIME_STRING_SIZE 64
#define DATE_TIME_STRING_SIZE 128

#define MAX_CB_CHANGE_FUNCTIONS 50
using dayChangeFunc = std::function<void(int, int)>;
using hourChangeFunc = std::function<void(int, int)>;
using minChangeFunc = std::function<void(int, int)>;
using secChangeFunc = std::function<void(int, int)>;
using uptimeChangeFunc = std::function<void(int, int, int, int)>;
using timeGetFunc = std::function<void()>;
class Time{
    public:
        void begin(boolean hasNetwork);
        void loop();

        void setTime(int year,int month,int day,int hour,int minute,int second);
        boolean setTimeFromStringObject(const char* timeString);
        void setTimeZone(const char* GMT_String);
        /* EVENT THINGS */
        void onDayChange(dayChangeFunc cb);
        void onMinChange(minChangeFunc cb);
        void onHourChange(hourChangeFunc cb);
        void onSecChange(secChangeFunc cb);
        void onUptimeChange(uptimeChangeFunc cb);
        void onTimeGet(timeGetFunc cb);
        /* EVENT THINGS */

        uint16_t getYear();
        uint8_t getMonth();
        uint8_t getDay();
        uint8_t getWeekDay();
        uint8_t getHour();
        uint8_t getMin();
        uint8_t getSec();
        const char* getMonthString();
        const char* getDayString();
        char* getTimeString();
        char* getDateTimeString();
        char* getUptimeString();

        uint32_t getEpoch(boolean inMilliseconds = false);
    private:
        boolean debug = true;
        boolean initialised = false;
        struct tm timeinfo;
        char uptimeString[25];
        char timeString[TIME_STRING_SIZE];
        char dateTimeString[DATE_TIME_STRING_SIZE];
        
        const char monthsOfTheYear[12][13]  = {"January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December"};
        const char daysOfTheWeek[7][13] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};
        const int daysInMonths[13]  = {31,28,31,30,31,30,31,31,30,31,30,31};
        const char* ntpServer   = "pool.ntp.org";

        /* for uptime hanling */
        long lastUptimeCheck = 0;
        int Up_Day  = 0,Up_Hour = 0,Up_Minute   = 0,Up_Second   = 0,HighMillis  = 0,Rollover        = 0,
            Month   = 0,Day     = 0,Hour        = 0,Min         = 0,Sec         = 0,currentDayInt   = 0;


        /* EVENT THINGS */
        int lastDay     = 0,
            lastHour    = 0,
            lastMin     = 0,
            lastSec     = 0;
        int currDayCbIndex  = 0,
            currHourCbIndex = 0,
            currMinCbIndex  = 0,
            currSecCbIndex  = 0,
            currUptimeCbIndex = 0,
            currTimeGetCbIndex = 0;
        void monitorDayChange();
        void monitorHourChange();
        void monitorMinChange();
        void monitorSecChange();
        dayChangeFunc dayChangeFunctions[MAX_CB_CHANGE_FUNCTIONS];
        hourChangeFunc hourChangeFunctions[MAX_CB_CHANGE_FUNCTIONS];
        minChangeFunc minChangeFunctions[MAX_CB_CHANGE_FUNCTIONS];
        secChangeFunc secChangeFunctions[MAX_CB_CHANGE_FUNCTIONS];
        uptimeChangeFunc uptimeChangeFuncs[MAX_CB_CHANGE_FUNCTIONS];
        timeGetFunc timeGetFuncs[MAX_CB_CHANGE_FUNCTIONS];

        void emitDayChange(int prev, int now);
        void emitHourChange(int prev, int now);
        void emitMinChange(int prev, int now);
        void emitSecChange(int prev, int now);
        void emitUptimeChange(int day,int hour, int min, int sec);
        void emitTimeGet();
        void handleEvents();
        /* EVENT THINGS */

        void handleUptime();
        void syncNTP();
        void updateTime();
};

inline Time hshTime;