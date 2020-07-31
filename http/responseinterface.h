#ifndef RESPONSEINTERFACE_H
#define RESPONSEINTERFACE_H
#include <QString>
#include <QtNetwork>

class ResponseInterface {
public:
    virtual bool isReady() = 0;
    virtual int getStatusCode() = 0;
    virtual QByteArray getBody() = 0;
    virtual void read();
};

#endif // RESPONSEINTERFACE_H
