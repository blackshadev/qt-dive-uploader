#ifndef DCUSBHIDDEVICE_H
#define DCUSBHIDDEVICE_H
#include "./dcdevice.h"
#include "../transports/dcusbhidtransport.h"
#include <libdivecomputer/usbhid.h>

class DCUSBHIDDevice : public DCDevice
{
public:
    DCUSBHIDDevice(DCContext *ctx, DCDeviceDescriptor *descr, dc_usbhid_device_t *dev);
    ~DCUSBHIDDevice();
    std::string getDescription();
protected:
    dc_usbhid_device_t *device;
    dc_iostream_t *loadNativeStream();
};

#endif // DCUSBHIDDEVICE_H
