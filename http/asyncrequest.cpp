#include "asyncrequest.h"

int iX = 0;

AsyncRequest::AsyncRequest(HTTPTransportInterface *trans)
    : QThread()
{
    index = iX++;
    transport = trans;
    response = new HTTPResponse(this);
}

AsyncRequest::~AsyncRequest()
{
    isDeleted = true;
    delete response;
    response = NULL;

    quit();
    wait();
}

void AsyncRequest::setAutoDelete(bool d)
{
    shouldAutoDelete = d;
}

void AsyncRequest::setMethod(RequestMethod m)
{
    method = m;
}

RequestMethod AsyncRequest::getMethod()
{
    return method;
}

QByteArray AsyncRequest::getQMethod()
{
    QByteArray verb;
    switch(method) {
        case RequestMethod::GET:
            verb = "GET";
        break;
        case RequestMethod::POST:
            verb = "POST";
        break;
        case RequestMethod::DELETE:
            verb = "DELETE";
        break;
        case RequestMethod::PUT:
            verb = "PUT";
        break;
        case RequestMethod::PATCH:
            verb = "PATCH";
        break;
    }
    return verb;
}

RequestState AsyncRequest::getState()
{
    return state;
}

void AsyncRequest::setURL(QString u)
{
    url = u;
}

QString AsyncRequest::getURL()
{
    return url;
}

void AsyncRequest::setHeader(QString header, QString content)
{
    headers.insert(header, content);
}

void AsyncRequest::setBody(QByteArray data)
{
    body = data;
}

void AsyncRequest::setBody(QJsonObject data)
{
    QJsonDocument doc(data);
    body = doc.toJson();
    setHeader("Content-Type", "application/json");
}

void AsyncRequest::beginSend()
{
    if (state != RequestState::None) {
        throw std::runtime_error("Already sending");
    }

    mutex.lock();
    setState(RequestState::Sending);
}

void AsyncRequest::endSend(QNetworkReply *nativeReply)
{
    setState(RequestState::Reading);
    connect(response, &HTTPResponse::finished, this,  [=]() {
        setState(RequestState::Completed);
        emit finished();
    });
    connect(response, &HTTPResponse::error, this,  [=](QString err) {
        setState(RequestState::Errorred);
        emit error(err);
    });
    response->readReply(nativeReply);
    mutex.unlock();
}

QByteArray AsyncRequest::getBody()
{
    return body;
}

HTTPResponse *AsyncRequest::getResponse()
{
    return response;
}

RequestInterface *AsyncRequest::getRequest()
{
    return this;
}

QNetworkRequest AsyncRequest::getNetworkRequest()
{
    QNetworkRequest req;
    req.setUrl(QUrl::fromUserInput(url));
    for (auto h : headers.keys()) {
        req.setRawHeader(h.toLocal8Bit(), headers[h].toLocal8Bit());
    }

    return req;
}

void AsyncRequest::abort()
{
    mutex.lock();
    if (response) {
        response->getNetworkReply()->abort();
    }
    setState(RequestState::Aborted);
    mutex.unlock();

    quit();
    wait();
}

void AsyncRequest::send()
{
    transport->sendRequest(this);
}

void AsyncRequest::run()
{
    send();
    exec();
}

void AsyncRequest::setState(RequestState s)
{
    state = s;
    emit stateChanged();
}

void AsyncRequest::autoDelete()
{

}
