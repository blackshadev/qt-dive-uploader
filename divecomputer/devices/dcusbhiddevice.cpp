#include "dcusbhiddevice.h"
#include "../../common/string_format.h"

DCUSBHIDDevice::DCUSBHIDDevice(DCContext *ctx, dc_usbhid_device_t *dev) : DCDevice(ctx)
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
    return string_format("PID: %u; VID: %u", pid, vid);
}

dc_iostream_t *DCUSBHIDDevice::loadNativeStream()
{
    dc_iostream_t *stream;
    dc_usbhid_open(&stream, context->getNative(), device);
    return stream;
}
