#include "datetime.h"


std::string format_datetime(dc_datetime_t &dt) {
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
        dt.timezone / 3600,
        dt.timezone % 3600
    );
    return std::string(buff);
}
