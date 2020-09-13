#ifndef DCIRDATRANSPORT_H
#define DCIRDATRANSPORT_H
#include "dctransport.h"

class DCIRDATransport : public DCTransport
{
public:
    DCIRDATransport(DCContextInterface *ctx);

    // DCTransportInterface interface
public:
    TransportType getTransportType();
    std::vector<DCDeviceInterface *> *getDevices(DCDeviceDescriptor *descriptor);
};

#endif // DCIRDATRANSPORT_H
