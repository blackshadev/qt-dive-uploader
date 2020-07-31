#ifndef REQUESTINTERFACE_H
#define REQUESTINTERFACE_H
#include <QString>
#include <QtNetwork>
#include "responseinterface.h"

enum RequestState {
    None,
    Sending,
    Reading,
    Completed
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
    virtual RequestState getState() = 0;
    virtual void setURL(QString url) = 0;
    virtual QString getURL() = 0;
    virtual void setHeader(QNetworkRequest::KnownHeaders header, QString content);
    virtual void setBody(QByteArray data);
    virtual void send() = 0;
    virtual void abort() = 0;
    virtual ResponseInterface getResponse();
};

#endif // REQUESTINTERFACE_H
