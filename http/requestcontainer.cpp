#include "requestcontainer.h"

RequestContainer::RequestContainer(QObject *parent)
    : QObject(parent)
{
    mainThreadId = QThread::currentThreadId();
}

RequestContainer::~RequestContainer()
{
    abortAll();
    cleanup();
}

void RequestContainer::sendRequest(RequestInternalInterface *request)
{
    if (isMainThread()) {
        send((AsyncRequest *)request);
        return;
    }

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
    if (isMainThread()) {
        request->start();
    } else {
        sendRequest(request);
    }
}

bool RequestContainer::isMainThread()
{
    return QThread::currentThreadId() == mainThreadId;
}

void RequestContainer::track(RequestInternalInterface *request)
{
    pendingRequests.insert(request);
}

void RequestContainer::untrack(RequestInternalInterface *request)
{
    pendingRequests.remove(request);
    if (request->getAutoDelete()) {
        trashheap.insert(request);
    }
}

void RequestContainer::cleanup()
{
    for (auto req : trashheap) {
        delete req;
    }
    trashheap.clear();
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
