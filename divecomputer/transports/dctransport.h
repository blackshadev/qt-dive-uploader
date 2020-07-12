#ifndef DCTRANSPORT_H
#define DCTRANSPORT_H
#include "dctransportinterface.h"
#include "../context/dccontext.h"

class DCTransport : public DCTransportInterface
{
public:
    DCTransport(DCContext *); 
protected:
    DCContext *context;
};

#endif // DCTRANSPORT_H
