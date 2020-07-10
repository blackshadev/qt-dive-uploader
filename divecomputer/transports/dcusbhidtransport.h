#ifndef DCUSBHIDTRANSPORT_H
#define DCUSBHIDTRANSPORT_H
#include "dctransportinterface.h"

class DCUSBHIDTransport : public DCTransportInterface
{
public:
    DCUSBHIDTransport(DCContext *ctx);
    TransportType getTransportType();
    virtual std::vector<DCDevice *> *getDevices(DCDeviceDescriptor *descriptor);
};

#endif // DCUSBHIDTRANSPORT_H
