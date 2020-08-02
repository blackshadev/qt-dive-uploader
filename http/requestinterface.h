#ifndef REQUESTINTERFACE_H
#define REQUESTINTERFACE_H
#include <QString>
#include <QtNetwork>
#include "responseinterface.h"

enum RequestState {
    None,
    Sending,
    Reading,
    Completed,
    Aborted,
    Errorred,
};

enum RequestMethod {
    GET = 1,
    POST = 2,
    PUT = 3,
    PATCH = 4,
    DELETE = 5
};

class RequestInterface {
public:
    virtual void setMethod(RequestMethod m) = 0;
    virtual RequestMethod getMethod() = 0;
    virtual QByteArray getQMethod() = 0;
    virtual RequestState getState() = 0;
    virtual void setURL(QString url) = 0;
    virtual QString getURL() = 0;
    virtual void setHeader(QString header, QString content) = 0;
    virtual void setBody(QByteArray data) = 0;
    virtual void setBody(QJsonObject data) = 0;
    virtual QByteArray getBody() = 0;
    virtual ResponseInterface *getResponse() = 0;
};

class RequestInternalInterface {
public:
    virtual void send() = 0;
    virtual void beginSend() = 0;
    virtual void endSend(QNetworkReply *repl) = 0;
    virtual QNetworkRequest getNetworkRequest() = 0;
    virtual void abort() = 0;
    virtual RequestInterface *getRequest() = 0;
};

#endif // REQUESTINTERFACE_H
