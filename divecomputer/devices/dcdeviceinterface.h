#ifndef DCDEVICEINTERFACE_H
#define DCDEVICEINTERFACE_H
#include "../descriptor/dcdevicedescriptor.h"
#include <libdivecomputer/device.h>

class DCDeviceInterface
{
public:
    DCDeviceDescriptor *getDescriptor() const;
    dc_device_t *getNative();
};

#endif // DCDEVICEINTERFACE_H
