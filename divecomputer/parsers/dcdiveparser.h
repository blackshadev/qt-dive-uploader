#ifndef DIVEPARSER_H
#define DIVEPARSER_H
#include <libdivecomputer/parser.h>
#include <libdivecomputer/descriptor.h>
#include "../entities/dcdive.h"
#include "../devices/dcdevice.h"
#include "../common.h"

class DiveParser
{
public:
    DiveParser();
    DiveParser *setDevice(DCDevice *device);
    DCDive *parseDive(rawdivedata_t &data);
protected:
    inline static datetime_t parseDatetime(dc_datetime_t datetime);
    inline static salinity_t parseSalinity(dc_salinity_t salinity);
    inline static tank_t parseTank(optional_t<dc_tank_t> pressure, optional_t<dc_gasmix_t> gasmix);
private:
    dc_parser_t *parser;
};

#endif // DIVEPARSER_H
