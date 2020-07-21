#ifndef DIVEPARSER_H
#define DIVEPARSER_H
#include <libdivecomputer/parser.h>
#include <libdivecomputer/descriptor.h>
#include "../entities/dcdive.h"
#include "../devices/dcdevice.h"
#include "../common.h"
#include "dcdivesampleparser.h"
#include "dcdiveparserinterface.h"

class DCDiveParser : public DCDiveParserInterface
{
public:
    DCDiveParser();
    virtual DCDive *parseDive(rawdivedata_t &data);
    virtual DCDiveParser *setDevice(DCDeviceInterface *device);
protected:
    inline static datetime_t parseDatetime(dc_datetime_t datetime);
    inline static salinity_t parseSalinity(dc_salinity_t salinity);
    inline static tank_t parseTank(optional_t<dc_tank_t> pressure, optional_t<dc_gasmix_t> gasmix);
    virtual DCDive *createDive();
    virtual DiveSampleParser *createSampleParser();
private:
    dc_parser_t *parser;
};

#endif // DIVEPARSER_H
