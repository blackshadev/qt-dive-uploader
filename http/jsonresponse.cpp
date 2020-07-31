#include "jsonresponse.h"

JSONResponse *JSONResponse::From(QNetworkReply *reply)
{
    auto response = new JSONResponse();

    response->reply = reply;
    reply->connect(reply, &QNetworkReply::finished, response, &JSONResponse::read);
    reply->connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), response, &JSONResponse::read);

    return response;
}

JSONResponse::JSONResponse(QObject *parent)
    : QObject(parent)
{

}

int JSONResponse::getStatusCode()
{
    if (!isReady()) {
        throw std::runtime_error("Response not ready");
    }
    return status;
}

QByteArray JSONResponse::getBody()
{
    if (!isReady()) {
        throw std::runtime_error("Response not ready");
    }
    return body;
}

void JSONResponse::read()
{
    auto replyData = reply->readAll();

    status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    body = replyData;

    QJsonParseError parseError;
    if (reply->error() != QNetworkReply::NetworkError::NoError) {
        setError(parseError.errorString());
        return;
    }

    jsonBody = QJsonDocument::fromJson(replyData, &parseError).object();
    if(parseError.error == QJsonParseError::NoError) {
        setError(parseError.errorString());
        return;
    }

    isRead = true;
    emit finished();
}

bool JSONResponse::isReady()
{
    return isRead;
}

QJsonObject JSONResponse::getBodyAsJSON()
{
    if (!isReady()) {
        throw std::runtime_error("Response not ready");
    }
    return jsonBody;
}

void JSONResponse::setError(QString err)
{
    error = err;
    hasError = true;
}



