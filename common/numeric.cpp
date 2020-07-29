#include "numeric.h"

std::string formatFloat(float num)
{
    char buff[20];
    std::sprintf(
        buff,
        "%.2f",
         num
    );
    return std::string(buff);
}
