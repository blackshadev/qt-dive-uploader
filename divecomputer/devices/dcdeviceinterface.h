#ifndef DCDEVICEINTERFACE_H
#define DCDEVICEINTERFACE_H
#include "../descriptor/dcdevicedescriptor.h"
#include <libdivecomputer/device.h>

class DCDeviceInterface
{
public:
    virtual DCDeviceDescriptor *getDescriptor() const = 0;
    virtual dc_device_t *getNative() = 0;
    virtual std::string getDescription() = 0;
};

#endif // DCDEVICEINTERFACE_H
