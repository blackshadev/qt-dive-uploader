#ifndef DCDEVICE_H
#define DCDEVICE_H
#include "dcdeviceinterface.h"
#include <libdivecomputer/device.h>

class DCDevice : public DCDeviceInterface
{
public:
    DCDevice(DCDeviceDescriptor *descr);
    virtual ~DCDevice();
    virtual std::string getDescription() override = 0;
    dc_device_t *getNative(DCContextInterface *ctx) override;

protected:
    virtual dc_iostream_t *getNativeStream(DCContextInterface *ctx);
    virtual dc_iostream_t *createNativeStream(DCContextInterface *ctx) = 0;
    DCDeviceDescriptor *descriptor;
    dc_iostream_t *iostream;
    dc_device_t *device;
};

#endif // DCDEVICE_H
