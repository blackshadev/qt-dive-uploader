#ifndef JSONRESPONSE_H
#define JSONRESPONSE_H
#include "responseinterface.h"

class HTTPResponse : public QObject, public ResponseInterface
{
    Q_OBJECT
public:
    HTTPResponse(QObject *parent = NULL);
    ~HTTPResponse();

    // ResponseInterface interface
public:

    void readReply(QNetworkReply *reply) override;
    int getStatusCode() override;
    QByteArray getBody() override;
    void read() override;
    bool isReady() override;
    bool hasError();
    QString errorString();

    QNetworkReply *getNetworkReply() override;
    QJsonValue getBodyAsJSON() override;

signals:
    void error(QString err);
    void finished();

protected:
    void setError(QString err);

private:
    int status = 0;
    bool isRead = false;

    QByteArray body;
    QJsonDocument jsonBody;
    QNetworkReply *reply = NULL;
    QString errMsg;
};

#endif // JSONRESPONSE_H
