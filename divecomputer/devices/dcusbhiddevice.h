#ifndef DCUSBHIDDEVICE_H
#define DCUSBHIDDEVICE_H
#include "./dcdevice.h"
#include "../transports/dcusbhidtransport.h"
#include <libdivecomputer/usbhid.h>

class DCUSBHIDDevice : public DCDevice
{
public:
    DCUSBHIDDevice(DCContext *ctx, dc_usbhid_device_t *dev);
    ~DCUSBHIDDevice();
    std::string getDescription();
    dc_iostream_t *loadNativeStream();
protected:
    dc_usbhid_device_t *device;
};

#endif // DCUSBHIDDEVICE_H
