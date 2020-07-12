#ifndef DCTRANSPORT_H
#define DCTRANSPORT_H
#include "dctransportinterface.h"
#include "../context/dccontext.h"

class DCTransport2 : public DCTransportInterface
{
public:
    DCTransport2(DCContext *);
protected:
    DCContext *context;
};

#endif // DCTRANSPORT_H
