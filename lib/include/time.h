#pragma once
#include <stdint.h>

namespace std
{
    enum class time_format : uint8_t { military, standard };

    typedef struct
    {
        uint16_t millis;
        uint8_t  second;
        uint8_t  minute;
        uint8_t  hour;  
        uint8_t  day;   
        uint8_t  wkday;
        uint8_t  month;
        uint16_t year;
    } PACKED time_t;

    time_t timenow();
    void   timenow(time_t* tm);
    char*  timestr(time_t tm, char* buff, time_format fmt, bool secs);
}