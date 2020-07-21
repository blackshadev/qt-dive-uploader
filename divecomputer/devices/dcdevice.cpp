#include "dcdevice.h"

DCDevice::DCDevice(DCContext *ctx, DCDeviceDescriptor *descr)
{
    context = ctx;
    descriptor = descr;
    stream = NULL;
    device = NULL;
}

DCDevice::~DCDevice()
{
    if(device) {
        dc_device_close(device);
        device = NULL;
    }
    if(stream) {
        dc_iostream_close(stream);
        stream = NULL;
    }
}

dc_iostream_t *DCDevice::getNativeStream()
{
    if (!this->stream) {
        stream = loadNativeStream();
    }

    return stream;
}

DCContext *DCDevice::getContext() const {
    return context;
}

DCDeviceDescriptor *DCDevice::getDescriptor() const
{
    return descriptor;
}

dc_device_t *DCDevice::getNative()
{
    if(device) {
        return device;
    }

    dc_device_open(&device, context->getNative(), descriptor->getNative(), getNativeStream());

    return device;
}

dc_device_t *DCDevice::testgetNative()
{
    return getNative();
}
