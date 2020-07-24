#ifndef DCUSBHIDTRANSPORT_H
#define DCUSBHIDTRANSPORT_H
#include "dctransport.h"

class DCUSBHIDTransport : public DCTransport
{
public:
    DCUSBHIDTransport(DCContextInterface *ctx);
    TransportType getTransportType();
    std::vector<DCDeviceInterface *> *getDevices(DCDeviceDescriptor *descriptor);
};

#endif // DCUSBHIDTRANSPORT_H
