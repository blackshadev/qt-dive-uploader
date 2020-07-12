#ifndef TRANSPORTINTERFACE_H
#define TRANSPORTINTERFACE_H
#include "transporttype.h"
#include "../devices/dcdeviceinterface.h"
#include <vector>

class DCTransportInterface
{
public:
    virtual TransportType getTransportType() = 0;
    virtual std::vector<DCDeviceInterface *> *getDevices(DCDeviceDescriptor *descriptor) = 0;
};

#endif // TRANSPORTINTERFACE_H
