#ifndef DCTRANSPORTFACTORY_H
#define DCTRANSPORTFACTORY_H
#include "dctransportinterface.h"

class DCTransportFactory
{
public:
    DCTransportFactory *setContext(DCContextInterface *ctx);
    virtual DCTransportInterface *create(TransportType type);
protected:
    DCContextInterface *context;
};

#endif // DCTRANSPORTFACTORY_H
