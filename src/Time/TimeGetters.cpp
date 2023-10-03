#include <Time/Time.h>


/*
 *   Return the year;
 */
uint16_t Time::getYear() {
    return timeinfo.tm_year + 1900;
}

/*
 *   Return the month;
 */
uint8_t Time::getMonth() {
    return timeinfo.tm_mon + 1;
}

/*
 *   Return the dayOfMonth;
 */
uint8_t Time::getDay() {
    return timeinfo.tm_mday;
}

/*
 *   Return the dayOfWeek;
 */
uint8_t Time::getWeekDay() {
    return timeinfo.tm_wday;
}

/*
 *   Return the hour;
 */
uint8_t Time::getHour() {
    return timeinfo.tm_hour;
}

/*
 *   Return the min;
 */
uint8_t Time::getMin() {
    return timeinfo.tm_min;
}

/*
 *   Return the sec;
 */
uint8_t Time::getSec() {
    return timeinfo.tm_sec;
}

/*
 *   Return the month string;
 */
const char* Time::getMonthString() {
    return monthsOfTheYear[timeinfo.tm_mon];
}

/*
 *   Return the day string;
 */
const char* Time::getDayString() {
    return daysOfTheWeek[timeinfo.tm_wday];
}

/*
 *   Getting the time string
 *   Format is: year month/dayOfMonth hour:min:sec
 */
char* Time::getTimeString() {
    sprintf(
        timeString, "%d %02d/%02d %02d:%02d:%02d",
        timeinfo.tm_year + 1900,
        timeinfo.tm_mon + 1,
        timeinfo.tm_mday,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec
    );
    return timeString;
}

/*
 *   Getting the date time string
 *   Format is: DayString, Month d, yyyy hour:min:sec
 */
char* Time::getDateTimeString() {
    sprintf(
        dateTimeString, "%s, %s %02d, %d %02d:%02d:%02d",
        getDayString(),
        getMonthString(),
        timeinfo.tm_mday,
        timeinfo.tm_year + 1900,
        timeinfo.tm_hour,
        timeinfo.tm_min,
        timeinfo.tm_sec
    );
    return dateTimeString;
}

/*
 *   Get the uptime string.
 *   Format: "%dD %02d:%02d:%02d"
 */
char* Time::getUptimeString() {
    return uptimeString;
}

