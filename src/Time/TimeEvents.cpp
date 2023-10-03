#include <Time/time.h>

/*
*   Emitting uptime change to every registered function.
*/
void Time::emitUptimeChange(int day,int hour, int min, int sec){
    for (size_t i = 0; i < MAX_CB_CHANGE_FUNCTIONS; i++){
        if(uptimeChangeFuncs[i]){
            uptimeChangeFuncs[i](day,hour,min,sec);
        }
    }
}

/*
*   Emitting day change to every registered function.
*/
void Time::emitDayChange(int prev, int now){
    // Preventing change on initialization.
    if( prev == 0 ){ return; }
    for (size_t i = 0; i < MAX_CB_CHANGE_FUNCTIONS; i++){
        if(dayChangeFunctions[i]){
            dayChangeFunctions[i](prev,now);
        }
    }
}

/*
*   Emitting hour change to every registered function.
*/
void Time::emitHourChange(int prev, int now){
    // Preventing change on initialization.
    if( prev == 0 ){ return; }
    for (size_t i = 0; i < MAX_CB_CHANGE_FUNCTIONS; i++){
        if(hourChangeFunctions[i]){
            hourChangeFunctions[i](prev,now);
        }
    }
}

/*
*   Emitting minute change to every registered function.
*/
void Time::emitMinChange(int prev, int now){
    // Preventing change on initialization.
    if( prev == 0 ){ return; }
    for (size_t i = 0; i < MAX_CB_CHANGE_FUNCTIONS; i++){
        if(minChangeFunctions[i]){
            minChangeFunctions[i](prev,now);
        }
    }
}

/*
*   Emitting second change to every registered function.
*/
void Time::emitSecChange(int prev, int now){
    // Preventing change on initialization.
    if( prev == 0 ){ return; }
    for (size_t i = 0; i < MAX_CB_CHANGE_FUNCTIONS; i++){
        if(secChangeFunctions[i]){
            secChangeFunctions[i](prev,now);
        }
    }
}

void Time::emitTimeGet(){
    for (size_t i = 0; i < MAX_CB_CHANGE_FUNCTIONS; i++){
        if(timeGetFuncs[i]){
            timeGetFuncs[i]();
        }
    }
}

/*
*   Registering a day change function.
*   Maximum function pointer count is MAX_CB_CHANGE_FUNCTIONS ( see timeSys.h )
*/
void Time::onDayChange( dayChangeFunc cb ){
    if( currDayCbIndex >= MAX_CB_CHANGE_FUNCTIONS ){
        if(debug){
            Serial.printf("[Time] - Can not register an other day change function.\n");
        }
        return;
    }
    dayChangeFunctions[currDayCbIndex] = cb;
    currDayCbIndex++;
}

/*
*   Registering a hour change function.
*   Maximum function pointer count is MAX_CB_CHANGE_FUNCTIONS ( see timeSys.h )
*/

void Time::onHourChange(hourChangeFunc cb){
    if( currHourCbIndex >= MAX_CB_CHANGE_FUNCTIONS ){
        if(debug){
            Serial.printf("[Time] - Can not register an other hour change function.\n");
        }
        return;
    }
    hourChangeFunctions[currHourCbIndex] = cb;
    currHourCbIndex++;
}

/*
*   Registering a minute change function.
*   Maximum function pointer count is MAX_CB_CHANGE_FUNCTIONS ( see timeSys.h )
*/
void Time::onMinChange(minChangeFunc cb){
    if( currMinCbIndex >= MAX_CB_CHANGE_FUNCTIONS ){
        if(debug){
            Serial.printf("[Time] - Can not register an other min change function.\n");
        }
        return;
    }
    minChangeFunctions[currMinCbIndex] = cb;
    currMinCbIndex++;
}

/*
*   Registering a second change function.
*   Maximum function pointer count is MAX_CB_CHANGE_FUNCTIONS ( see timeSys.h )
*/
void Time::onSecChange(minChangeFunc cb){
    if( currSecCbIndex >= MAX_CB_CHANGE_FUNCTIONS ){
        if(debug){
            Serial.printf("[Time] - Can not register an other sec change function.\n");
        }
        return;
    }
    secChangeFunctions[currSecCbIndex] = cb;
    currSecCbIndex++;
}

void Time::onUptimeChange(uptimeChangeFunc cb){
    if( currUptimeCbIndex >= MAX_CB_CHANGE_FUNCTIONS ){
        if(debug){
            Serial.printf("[Time] - Can not register an other uptime change function.\n");
        }
        return;
    }
    uptimeChangeFuncs[currUptimeCbIndex] = cb;
    currUptimeCbIndex++;
}

void Time::onTimeGet(timeGetFunc cb){
    if( currTimeGetCbIndex >= MAX_CB_CHANGE_FUNCTIONS ){
        if(debug){
            Serial.printf("[Time] - Can not register an other timeGet function.\n");
        }
        return;
    }
    timeGetFuncs[currTimeGetCbIndex] = cb;
    currTimeGetCbIndex++;
}
/*
 *   Monitoring the day change
 *   Calling emitDayChange() ( see timeSysEvents.cpp )
 */
void Time::monitorDayChange() {
    if (lastDay == timeinfo.tm_mday) {
        return;
    }
    emitDayChange(lastDay, timeinfo.tm_mday);
    lastDay = timeinfo.tm_mday;
}

/*
 *   Monitoring the hour change
 *   Calling emitHourChange() ( see timeSysEvents.cpp )
 */
void Time::monitorHourChange() {
    if (lastHour == timeinfo.tm_hour) {
        return;
    }
    emitHourChange(lastHour, timeinfo.tm_hour);
    lastHour = timeinfo.tm_hour;
}

/*
 *   Monitoring the minute change
 *   Calling emitMinChange() ( see timeSysEvents.cpp )
 */
void Time::monitorMinChange() {
    if (lastMin == timeinfo.tm_min) {
        return;
    }
    emitMinChange(lastMin, timeinfo.tm_min);
    lastMin = timeinfo.tm_min;
}

/*
 *   Monitoring the second change
 *   Calling emitSecChange() ( see timeSysEvents.cpp )
 */
void Time::monitorSecChange() {
    if (lastSec == timeinfo.tm_sec) {
        return;
    }
    emitSecChange(lastSec, timeinfo.tm_sec);
    lastSec = timeinfo.tm_sec;
}

void Time::handleEvents(){
    monitorDayChange();
    monitorHourChange();
    monitorMinChange();
    monitorSecChange();
}