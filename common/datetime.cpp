#include "datetime.h"


std::string format_datetime_iso(dc_datetime_t &dt) {
    char buff[25];
    std::sprintf(
        buff,
        "%04d-%02d-%02dT%02d:%02d:%02d+%02d:%02d",
        dt.year,
        dt.month,
        dt.day,
        dt.hour,
        dt.minute,
        dt.second,
        GET_TIMEZONE(dt.timezone) / 3600,
        GET_TIMEZONE(dt.timezone) % 3600
    );
    return std::string(buff);
}

std::string format_datetime_display(dc_datetime_t &dt) {

    char buff[25];
    std::sprintf(
        buff,
        "%04d-%02d-%02d %02d:%02d:%02d",
        dt.year,
        dt.month,
        dt.day,
        dt.hour + GET_TIMEZONE(dt.timezone) / 3600,
        dt.minute + GET_TIMEZONE(dt.timezone) % 3600,
        dt.second
    );
    return std::string(buff);
}
