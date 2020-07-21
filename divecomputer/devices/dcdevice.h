#ifndef DCDEVICE_H
#define DCDEVICE_H
#include "../context/dccontext.h"
#include "dcdeviceinterface.h"
#include <libdivecomputer/device.h>

class DCDevice : public DCDeviceInterface
{
public:
    DCDevice(DCContext *ctx, DCDeviceDescriptor *descr);
    virtual ~DCDevice();
    virtual std::string getDescription() override = 0;
    DCContext *getContext() const;
    DCDeviceDescriptor *getDescriptor() const override;
    dc_device_t *getNative() override;
    dc_device_t *testgetNative() override;
protected:
    DCContext *context;
    DCDeviceDescriptor *descriptor;
    dc_iostream_t *stream;
    dc_device_t *device;
    virtual dc_iostream_t *loadNativeStream() = 0;
    dc_iostream_t *getNativeStream();
};

#endif // DCDEVICE_H
