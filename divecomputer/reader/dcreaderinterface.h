#ifndef DCREADERINTERFACE_H
#define DCREADERINTERFACE_H
#include "../entities/dcdive.h"
#include "../devices/dcdeviceinterface.h"
#include "../parsers/dcdiveparserinterface.h"
#include "../context/dccontextinterface.h"

class DCReaderInterface
{
    virtual DCReaderInterface *setDevice(DCDeviceInterface *device) = 0;
    virtual DCReaderInterface *setParser(DCDiveParserInterface *parser) = 0;
    virtual DCReaderInterface *setContext(DCContextInterface *ctx)  = 0;
    virtual void startReading() = 0;
    virtual void setFingerprint(fingerprint_t fp) = 0;
    virtual void clearFingerprint() = 0;
    virtual void cancel() = 0;
};

#endif // DCREADERINTERFACE_H
