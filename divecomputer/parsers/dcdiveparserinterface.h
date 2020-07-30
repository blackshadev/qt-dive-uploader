#ifndef DCDIVEPARSERINTERFACE_H
#define DCDIVEPARSERINTERFACE_H
#include "../devices/dcdeviceinterface.h"
#include "../entities/dcdive.h"
#include "../data/rawdivedata.h"

class DCDiveParserInterface
{
public:
    virtual DCDiveParserInterface *setDevice(DCDeviceInterface *device) = 0;
    virtual DCDiveParserInterface *setContext(DCContextInterface *context) = 0;
    virtual DCDive *parseDive(rawdivedata_t &data) = 0;

};

#endif // DCDIVEPARSERINTERFACE_H
