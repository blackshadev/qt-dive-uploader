#include "dcserialdevice.h"

DCSerialDevice::DCSerialDevice(DCDeviceDescriptor *descr, dc_serial_device_t *dev) : DCDevice(descr)
{
    device = dev;
}

DCSerialDevice::~DCSerialDevice()
{
    dc_serial_device_free(device);
}

std::string DCSerialDevice::getDescription()
{
    char buff[256];
    snprintf(buff, 256, "%s\n", dc_serial_device_get_name(device));
    return std::string(buff);
}

dc_iostream_t *DCSerialDevice::createNativeStream(DCContextInterface *ctx)
{
    auto addr = dc_serial_device_get_name(device);
    dc_iostream_t *stream;
    dc_serial_open(&stream, ctx->getNative(), addr);
    return stream;
}
