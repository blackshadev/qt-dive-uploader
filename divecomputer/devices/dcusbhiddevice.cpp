#include "dcusbhiddevice.h"

DCUSBHIDDevice::DCUSBHIDDevice(DCDeviceDescriptor *descr, dc_usbhid_device_t *dev) : DCDevice(descr)
{
    device = dev;
}

DCUSBHIDDevice::~DCUSBHIDDevice()
{
    dc_usbhid_device_free(device);
}

std::string DCUSBHIDDevice::getDescription()
{
    auto pid = dc_usbhid_device_get_pid(device);
    auto vid = dc_usbhid_device_get_vid(device);

    char buff[256];
    snprintf(buff, 256, "PID: %u; VID: %u", pid, vid);
    return std::string(buff);
}

dc_iostream_t *DCUSBHIDDevice::createNativeStream(DCContextInterface *ctx)
{
    dc_iostream_t *stream;
    dc_usbhid_open(&stream, ctx->getNative(), device);
    return stream;
}
