#ifndef DCSERIALTRANSPORT_H
#define DCSERIALTRANSPORT_H
#include "dctransport.h"

class DCSerialTransport : public DCTransport
{
public:
    DCSerialTransport(DCContextInterface *ctx);

    // DCTransportInterface interface
public:
    TransportType getTransportType() override;
    std::vector<DCDeviceInterface *> *getDevices(DCDeviceDescriptor *descriptor) override;
};

#endif // DCSERIALTRANSPORT_H
