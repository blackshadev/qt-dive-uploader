#include "httpresponse.h"
#include "exceptions.h"


HTTPResponse::HTTPResponse(QObject *parent)
    : QObject(parent)
{
}

HTTPResponse::~HTTPResponse()
{
    reply->deleteLater();
    reply = NULL;
}

void HTTPResponse::readReply(QNetworkReply *rpl)
{
    reply = rpl;
    reply->connect(reply, &QNetworkReply::finished, this, &HTTPResponse::read);
    reply->connect(reply, &QNetworkReply::errorOccurred, this, &HTTPResponse::read);
}

int HTTPResponse::getStatusCode()
{
    if (!isReady()) {
        throw std::runtime_error("Response not ready");
    }
    return status;
}

QByteArray HTTPResponse::getBody()
{
    if (!isReady()) {
        throw std::runtime_error("Response not ready");
    }
    return body;
}

void HTTPResponse::read()
{
    if (isRead) {
        return;
    }

    auto errCode = reply->error();
    if (errCode != QNetworkReply::NoError &&  errCode < QNetworkReply::ContentAccessDenied) {
        setError(reply->errorString());
        return;
    }

    auto replyData = reply->readAll();

    status = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    body = replyData;

    isRead = true;
    emit finished();
}

bool HTTPResponse::isReady()
{
    return isRead;
}

bool HTTPResponse::hasError()
{
    return !errMsg.isEmpty();
}

QString HTTPResponse::errorString()
{
    return errMsg;
}

QNetworkReply *HTTPResponse::getNetworkReply()
{
    return reply;
}

QJsonValue HTTPResponse::getBodyAsJSON()
{
    if (!isReady()) {
        throw std::runtime_error("Response not ready");
    }

    QJsonParseError parseError;
    jsonBody = QJsonDocument::fromJson(body, &parseError);
    if(parseError.error != QJsonParseError::NoError) {
        setError(parseError.errorString());
        return QJsonValue::Null;
    }

    if (jsonBody.isArray()) {
        return QJsonValue(jsonBody.array());
    }
    if (jsonBody.isObject()) {
        return QJsonValue(jsonBody.object());
    }

    return QJsonValue::Null;
}

void HTTPResponse::setError(QString err)
{
    errMsg = err;
    emit error(err);
}



