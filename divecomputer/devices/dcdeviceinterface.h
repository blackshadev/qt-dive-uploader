#ifndef DCDEVICEINTERFACE_H
#define DCDEVICEINTERFACE_H
#include "../context/dccontextinterface.h"
#include "../descriptor/dcdevicedescriptor.h"
#include <libdivecomputer/device.h>

class DCDeviceInterface
{
public:
    virtual DCDeviceDescriptor *getDescriptor() const = 0;
    virtual dc_device_t *getNative(DCContextInterface *ctx) = 0;
    virtual std::string getDescription() = 0;
};

#endif // DCDEVICEINTERFACE_H
