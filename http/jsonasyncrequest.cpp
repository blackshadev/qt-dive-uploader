#include "jsonasyncrequest.h"

JsonAsyncRequest::JsonAsyncRequest(RequestContainerInterface *con)
    : QRunnable()
{
    container = con;
}

void JsonAsyncRequest::setMethod(RequestMethod m)
{
    method = m;
}

RequestMethod JsonAsyncRequest::getMethod()
{
    return method;
}

RequestState JsonAsyncRequest::getState()
{
    return state;
}

void JsonAsyncRequest::setURL(QString u)
{
    url = u;
}

QString JsonAsyncRequest::getURL()
{
    return url;
}

void JsonAsyncRequest::setHeader(QNetworkRequest::KnownHeaders header, QString content)
{
    request.setHeader(header, QVariant::fromValue(content));
}

void JsonAsyncRequest::setBody(QByteArray data)
{
    body = data;
}

void JsonAsyncRequest::setBody(QJsonObject data)
{
    QJsonDocument doc(data);
    body = doc.toJson();
}

void JsonAsyncRequest::send()
{
    if(state != RequestState::None) {
        throw std::runtime_error("Request already send");
    }
    container->track(this);

    state = RequestState::Sending;
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

    request.setUrl(QUrl::fromUserInput(url));
    setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    auto nativeReply = container->getNetworkAccessManager()->sendCustomRequest(request, verb, body);

    state = RequestState::Reading;
    response = JSONResponse::From(nativeReply);

    state = RequestState::Completed;
}

void JsonAsyncRequest::abort()
{

}
