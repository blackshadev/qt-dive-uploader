#ifndef ASYNCREQUEST_H
#define ASYNCREQUEST_H
#include <QRunnable>
#include "requestinterface.h"
#include "httpresponse.h"
#include <QtNetwork>
#include <QNetworkReply>
#include "httptransportinterface.h"

class AsyncRequest :  public QObject, public RequestInterface, public RequestInternalInterface
{
    Q_OBJECT
public:
    AsyncRequest(HTTPTransportInterface *transport);
    ~AsyncRequest();

    void done();
    bool shouldDelete() override;

    // RequestInterface interface
    void setMethod(RequestMethod m) override;
    RequestMethod getMethod() override;
    QByteArray getQMethod() override;
    RequestState getState() override;
    void setURL(QString url) override;
    QString getURL() override;
    void setHeader(QString header, QString content) override;
    void setBody(QByteArray data) override;
    void setBody(QJsonObject data) override;
    void abort() override;
    void send() override;
    void beginSend() override;
    void endSend(QNetworkReply *nativeReply) override;
    QNetworkRequest getNetworkRequest() override;
    QByteArray getBody() override;
    HTTPResponse *getResponse() override;
    RequestInterface *getRequest() override;

signals:
    void error(QString);
    void finished();
    void stateChanged(RequestState state);

protected:
    void setState(RequestState state);

private:
    int index = 0;
    bool isDeleted = false;
    bool shouldBeDelete = false;
    RequestMethod method = RequestMethod::GET;
    RequestState state = RequestState::None;
    QString url;
    QMap<QString, QString> headers;
    QByteArray body;
    HTTPResponse *response = NULL;
    HTTPTransportInterface *transport = NULL;
};

#endif // ASYNCREQUEST_H
