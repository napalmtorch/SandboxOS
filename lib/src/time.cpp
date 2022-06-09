#include <time.h>
#include <core/kernel.h>

namespace std
{
    time_t timenow() { return *os::hal::rtc::time(); }

    void timenow(time_t* tm)
    {
        if (tm == NULL) { return; }
        memcpy(tm, os::hal::rtc::time(), sizeof(time_t));
    }

    char* timestr(time_t tm, char* buff, time_format fmt, bool secs)
    {
        // temp vars
        char temp[64];
        memset(temp, 0, sizeof(temp));

        // 24-hour
        if (fmt == time_format::military)
        {
            if (tm.hour < 10) { strcat(buff, "0"); } 
            strcat(buff, ltoa(tm.hour, temp, 10));
        }
        // 12-hour
        else
        {
            int hour = tm.hour;
            if (hour > 12) { hour -= 12; }
            if (hour == 0) { hour = 12; }
            if (hour < 10) { strcat(buff, "0"); }
            strcat(buff, ltoa(hour, temp, 10));
        }
        
        // clear temp and add colon
        memset(temp, 0, sizeof(temp));
        strcat(buff, ":");

        // minute
        if (tm.minute < 10) { strcat(buff, "0"); }
        strcat(buff, ltoa(tm.minute, temp, 10));

        // clear temp
        memset(temp, 0, sizeof(temp));

        // seconds
        if (secs)
        {
            strcat(buff, ":");
            if (tm.second < 10) { strcat(buff, "0"); }
            strcat(buff, ltoa(tm.second, temp, 10));
        }

        // am/pm
        if (fmt == time_format::standard) { if (tm.hour >= 12) { strcat(buff, " PM"); } else { strcat(buff, " AM"); } }

        // return output string
        return buff;
    }
}