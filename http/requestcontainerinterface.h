#ifndef REQUESTCONTAINERINTERFACE_H
#define REQUESTCONTAINERINTERFACE_H
#include "requestinterface.h"
#include "httptransportinterface.h"

class RequestContainerInterface {
public:
    virtual void abortAll() = 0;
    virtual QNetworkAccessManager *getNetworkAccessManager() = 0;
};

#endif // REQUESTCONTAINERINTERFACE_H
