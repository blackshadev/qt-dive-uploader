#ifndef DCDEVICEINTERFACE_H
#define DCDEVICEINTERFACE_H
#include "../context/dccontextinterface.h"
#include "../descriptor/dcdevicedescriptor.h"
#include <libdivecomputer/device.h>

struct DeviceData {
public:
    unsigned int model;
    unsigned int serial;
    unsigned int firmware;
};

class DCDeviceInterface
{
public:
    virtual dc_device_t *getNative(DCContextInterface *ctx) = 0;
    virtual std::string getDescription() = 0;
};

#endif // DCDEVICEINTERFACE_H
