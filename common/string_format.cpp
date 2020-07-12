#include "string_format.h"
#include <string>
#include <stdio.h>

template<typename ... Args>
std::string string_format(const std::string& fmt, Args ...args) {
    const char* buff[256];
    snprintf(buff, sizeof(buff), fmt, args... );
    std::string buffAsStdStr((const char *)buff);
    return buffAsStdStr;
}
