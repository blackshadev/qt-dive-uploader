#ifndef DCTRANSPORT_H
#define DCTRANSPORT_H
#include "dctransportinterface.h"
#include "../context/dccontextinterface.h"

class DCTransport : public DCTransportInterface
{
public:
    DCTransport(DCContextInterface *);
protected:
    DCContextInterface *context;
};

#endif // DCTRANSPORT_H
