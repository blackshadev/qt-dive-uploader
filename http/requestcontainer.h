#ifndef REQUESTCONTAINER_H
#define REQUESTCONTAINER_H
#include <QObject>
#include "requestcontainerinterface.h"
#include "asyncrequest.h"

class RequestContainer : public QObject, public RequestContainerInterface, public HTTPTransportInterface
{
public:
    RequestContainer(QObject *parent);
    ~RequestContainer();

    // RequestContainerInterface interface
public:    
    virtual void sendRequest(RequestInternalInterface *request) override;
    virtual AsyncRequest *request();
    virtual void send(RequestInterface *req) override;
    virtual void send(AsyncRequest *request);
    bool isMainThread() override;

    void abortAll() override;
    QNetworkAccessManager *getNetworkAccessManager() override;
protected:
    void track(RequestInternalInterface *request);
    void untrack(RequestInternalInterface *request);
private:
    QNetworkAccessManager networkAccessManager;
    QSet<RequestInternalInterface *> pendingRequests;
    Qt::HANDLE mainThreadId;
};

#endif // REQUESTCONTAINER_H
