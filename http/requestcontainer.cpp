#include "requestcontainer.h"

RequestContainer::RequestContainer(QObject *parent)
    : QObject(parent)
{
}

RequestContainer::~RequestContainer()
{
    abortAll();
    cleanup();
}

void RequestContainer::sendRequest(RequestInternalInterface *request)
{
    request->beginSend();
    track(request);
    QNetworkAccessManager *nam = new QNetworkAccessManager();
    auto reply = nam->sendCustomRequest(request->getNetworkRequest(), request->getRequest()->getQMethod(), request->getRequest()->getBody());
    request->endSend(reply);
    connect(reply, &QNetworkReply::finished, this, [=]() {
        untrack(request);
    });
}

AsyncRequest *RequestContainer::request()
{
    return new AsyncRequest(this);
}

void RequestContainer::send(RequestInterface *req)
{
    send((AsyncRequest *)req);
}

void RequestContainer::send(AsyncRequest *request)
{
    request->send();
}


void RequestContainer::track(RequestInternalInterface *request)
{
    pendingRequests.insert(request);
}

void RequestContainer::untrack(RequestInternalInterface *request)
{
    pendingRequests.remove(request);
    trashheap.insert(request);
}

void RequestContainer::cleanup()
{
    for (auto req : trashheap) {
        if(req->shouldDelete()) {
            trashheap.remove(req);
            delete req;
        }
    }
}

void RequestContainer::abortAll()
{
    for (auto req : pendingRequests) {
        req->abort();
    }
}

QNetworkAccessManager *RequestContainer::getNetworkAccessManager()
{
    return &networkAccessManager;
}
