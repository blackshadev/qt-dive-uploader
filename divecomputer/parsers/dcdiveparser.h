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
    virtual ~DCDiveParser();
    virtual DCDive *parseDive(rawdivedata_t &data) override;
    virtual DCDiveParser *setDevice(DCDeviceInterface *device) override;
    virtual DCDiveParser *setContext(DCContextInterface *context) override;
protected:
    inline static datetime_t parseDatetime(dc_datetime_t datetime);
    inline static salinity_t parseSalinity(dc_salinity_t salinity);
    inline static tank_t parseTank(optional_t<dc_tank_t> pressure, optional_t<dc_gasmix_t> gasmix);
    virtual DCDive *createDive();
    virtual DiveSampleParser *createSampleParser();
    virtual void initialize();
    virtual void finalize(DCDive *dive);
    DiveSampleParser *sampleParser;
    DCDeviceInterface *device;
    DCContextInterface *context;
    dc_parser_t *parser;
    bool initialized = false;
private:
    void fixTankPressuresFromSamples(DCDive *dive);
    void reset();
};

#endif // DIVEPARSER_H
