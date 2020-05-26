#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <libdivecomputer/datetime.h>

#define GET_TIMEZONE(X) ((X == DC_TIMEZONE_NONE) ? 0 : X)

std::string format_datetime_iso(dc_datetime_t &dt);
std::string format_datetime_display(dc_datetime_t &dt);

#endif // DATETIME_H
