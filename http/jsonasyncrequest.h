#ifndef JSONASYNCREQUEST_H
#define JSONASYNCREQUEST_H
#include <QRunnable>
#include "requestinterface.h"
#include "requestcontainerinterface.h"
#include "jsonresponse.h"
#include <QtNetwork>
#include <QNetworkReply>

class JsonAsyncRequest : public QRunnable, public RequestInterface
{
public:
    JsonAsyncRequest(RequestContainerInterface *container);

    // RequestInterface interface
    void setMethod(RequestMethod m) override;
    RequestMethod getMethod() override;
    RequestState getState() override;
    void setURL(QString url) override;
    QString getURL() override;
    void setHeader(QNetworkRequest::KnownHeaders header, QString content) override;
    void setBody(QByteArray data) override;
    void setBody(QJsonObject data);
    void send() override;
    void abort() override;

    // QRunnable interface
protected:
    void run() override;
private:
    RequestContainerInterface *container;
    RequestMethod method = RequestMethod::GET;
    RequestState state = RequestState::None;
    QString url;
    QNetworkRequest request;
    QByteArray body;
    JSONResponse *response = NULL;
};

#endif // JSONASYNCREQUEST_H
