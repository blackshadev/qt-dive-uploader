#ifndef TRANSPORTINTERFACE_H
#define TRANSPORTINTERFACE_H
#include "transporttype.h"
#include "../context/dccontext.h"
#include <vector>

class DCTransportInterface
{
public:
    DCTransportInterface(DCContext *ctx);
    virtual ~DCTransportInterface();
    virtual TransportType getTransportType() = 0;
    virtual std::vector<DCDevice *> *getDevices() = 0;
protected:
    DCContext *context;
};

#endif // TRANSPORTINTERFACE_H
