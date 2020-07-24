#ifndef DCUSBHIDDEVICE_H
#define DCUSBHIDDEVICE_H
#include "./dcdevice.h"
#include "../transports/dcusbhidtransport.h"
#include <libdivecomputer/usbhid.h>

class DCUSBHIDDevice : public DCDevice
{
public:
    DCUSBHIDDevice(DCDeviceDescriptor *descr, dc_usbhid_device_t *dev);
    ~DCUSBHIDDevice();
    std::string getDescription() override;
protected:
    dc_usbhid_device_t *device;
    dc_iostream_t *createNativeStream(DCContextInterface *ctx) override;
};

#endif // DCUSBHIDDEVICE_H
