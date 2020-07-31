#ifndef JSONRESPONSE_H
#define JSONRESPONSE_H
#include "jsonresponseinterface.h"

class JSONResponse : public QObject, public JSONResponseInterface
{
public:
    static JSONResponse *From(QNetworkReply *reply);
    JSONResponse(QObject *parent = NULL);

    // ResponseInterface interface
public:
    int getStatusCode() override;
    QByteArray getBody() override;
    void read() override;
    bool isReady() override;

    // JSONResponseInterface interface
public:
    QJsonObject getBodyAsJSON() override;

public signals:
    void finished();

protected:
    void setError(QString err);

private:
    int status = 0;
    bool isRead = false;
    bool hasError = false;

    QByteArray body;
    QJsonObject jsonBody;
    QNetworkReply *reply;
    QString error;
};

#endif // JSONRESPONSE_H
