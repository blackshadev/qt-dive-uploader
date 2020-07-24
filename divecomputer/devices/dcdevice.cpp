#include "dcdevice.h"

DCDevice::DCDevice(DCDeviceDescriptor *descr)
{
    descriptor = descr;
    iostream = NULL;
    device = NULL;
}

DCDevice::~DCDevice()
{
    if (!iostream) {
        dc_iostream_close(iostream);
    }
    if (!device) {
        dc_device_close(device);
    }
}

DCDeviceDescriptor *DCDevice::getDescriptor() const
{
    return descriptor;
}

dc_device_t *DCDevice::getNative(DCContextInterface *ctx)
{
    if (!device) {
        dc_device_open(&device, ctx->getNative(), descriptor->getNative(), getNativeStream(ctx));
    }

    return device;
}

dc_iostream_t *DCDevice::getNativeStream(DCContextInterface *ctx)
{
    if (!iostream) {
        iostream = createNativeStream(ctx);
    }

    return iostream;
}

