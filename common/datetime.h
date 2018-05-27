#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <libdivecomputer/datetime.h>

std::string format_datetime_iso(dc_datetime_t &dt);
std::string format_datetime_display(dc_datetime_t &dt);

#endif // DATETIME_H
