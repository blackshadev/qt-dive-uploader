#ifndef REQUESTCONTAINERINTERFACE_H
#define REQUESTCONTAINERINTERFACE_H
#include "requestinterface.h"

class RequestContainerInterface {
public:
    virtual void track(RequestInterface *request) = 0;
    virtual void untrack(RequestInterface *request) = 0;
    virtual void abortAll() = 0;
    virtual QNetworkAccessManager *getNetworkAccessManager() = 0;
};

#endif // REQUESTCONTAINERINTERFACE_H
