#ifndef DIVEPARSER_H
#define DIVEPARSER_H
#include <libdivecomputer/parser.h>
#include <libdivecomputer/descriptor.h>
#include "../entities/dcdive.h"

class IDevice {
public:
    virtual dc_device_t *getNative() = 0;
};

class DiveParser
{
public:
    DiveParser();
    void setDevice(IDevice* device);
    DCDive *parseDives(const unsigned char* data, unsigned int size);
protected:
    inline static datetime_t parseDatetime(dc_datetime_t datetime);
    inline static salinity_t parseSalinity(dc_salinity_t salinity);
    inline static tank_t parseTank(optional_t<dc_tank_t> pressure, optional_t<dc_gasmix_t> gasmix);
private:
    dc_parser_t *parser;
};

#endif // DIVEPARSER_H
