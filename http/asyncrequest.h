#ifndef ASYNCREQUEST_H
#define ASYNCREQUEST_H
#include <QRunnable>
#include "requestinterface.h"
#include "httpresponse.h"
#include <QtNetwork>
#include <QNetworkReply>
#include "httptransportinterface.h"

class AsyncRequest :  public QThread, public RequestInterface, public RequestInternalInterface
{
    Q_OBJECT
public:
    AsyncRequest(HTTPTransportInterface *transport);
    ~AsyncRequest();

    void setAutoDelete(bool d);

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
    void stateChanged();

    // QRunnable interface
protected:
    void run() override;
    void setState(RequestState state);
    void autoDelete();
private:
    bool shouldAutoDelete = true;
    QMutex mutex;
    RequestMethod method = RequestMethod::GET;
    RequestState state = RequestState::None;
    QString url;
    QMap<QString, QString> headers;
    QByteArray body;
    HTTPResponse *response;
    HTTPTransportInterface *transport;

};

#endif // ASYNCREQUEST_H
