#ifndef HTTPTRANSPORTINTERFACE_H
#define HTTPTRANSPORTINTERFACE_H
#include "requestinterface.h"

class HTTPTransportInterface
{
public:
    virtual void sendRequest(RequestInternalInterface *req) = 0;
    virtual void send(RequestInterface *req) = 0;
    virtual bool isMainThread() = 0;
};

#endif // HTTPTRANSPORTINTERFACE_H
