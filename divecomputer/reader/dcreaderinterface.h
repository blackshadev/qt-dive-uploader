#ifndef DCREADERINTERFACE_H
#define DCREADERINTERFACE_H
#include "../entities/dcdive.h"
#include "../devices/dcdeviceinterface.h"
#include "../parsers/dcdiveparserinterface.h"

class DCReaderInterface
{
    virtual DCReaderInterface *setDevice(DCDeviceInterface *device) = 0;
    virtual DCReaderInterface *setParser(DCDiveParserInterface *parser) = 0;
    virtual void start() = 0;
};

#endif // DCREADERINTERFACE_H
