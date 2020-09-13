#include "dcirdadevice.h"

DCIRDADevice::DCIRDADevice(DCDeviceDescriptor *descr, dc_irda_device_t *dev) : DCDevice(descr)
{
    device = dev;
}

DCIRDADevice::~DCIRDADevice()
{
    dc_irda_device_free(device);
}

std::string DCIRDADevice::getDescription()
{
    char buff[256];
    snprintf(buff, 256, "%08x\t%s\n", dc_irda_device_get_address (device), dc_irda_device_get_name (device));
    return std::string(buff);
}

dc_iostream_t *DCIRDADevice::createNativeStream(DCContextInterface *ctx)
{
    auto addr = dc_irda_device_get_address(device);
    dc_iostream_t *stream;
    dc_irda_open(&stream, ctx->getNative(), addr, 1);
    return stream;
}
