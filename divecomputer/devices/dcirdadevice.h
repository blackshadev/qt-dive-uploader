#ifndef DCIRDADEVICE_H
#define DCIRDADEVICE_H
#include "dcdevice.h"
#include <libdivecomputer/irda.h>

class DCIRDADevice : public DCDevice
{
public:
    DCIRDADevice(DCDeviceDescriptor *descr, dc_irda_device_t *dev);
    ~DCIRDADevice();

    // DCDeviceInterface interface
public:
    std::string getDescription();

    // DCDevice interface
protected:
    dc_irda_device_t *device;
    dc_iostream_t *createNativeStream(DCContextInterface *ctx);
};

#endif // DCIRDADEVICE_H
