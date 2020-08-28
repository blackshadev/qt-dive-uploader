#ifndef REQUESTCONTAINER_H
#define REQUESTCONTAINER_H
#include <QObject>
#include "requestcontainerinterface.h"
#include "asyncrequest.h"

class RequestContainer : public QObject, public RequestContainerInterface, public HTTPTransportInterface
{
    Q_OBJECT
public:
    RequestContainer(QObject *parent = NULL);
    ~RequestContainer();

    // RequestContainerInterface interface
public:    
    virtual void sendRequest(RequestInternalInterface *request) override;
    virtual AsyncRequest *request();
    virtual void send(RequestInterface *req) override;
    virtual void send(AsyncRequest *request);
    void abortAll() override;
    QNetworkAccessManager *getNetworkAccessManager() override;

public slots:
    void cleanup();

protected:
    void track(RequestInternalInterface *request);
    void untrack(RequestInternalInterface *request);
private:
    QNetworkAccessManager networkAccessManager;
    QSet<RequestInternalInterface *> pendingRequests;
    QSet<RequestInternalInterface *> *trashheap;
};
Q_DECLARE_METATYPE(RequestContainer *)

#endif // REQUESTCONTAINER_H
