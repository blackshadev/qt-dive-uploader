#ifndef JSONREQUEST_H
#define JSONREQUEST_H
#include <QObject>
#include <QtNetwork>
#include <QJsonDocument>
#include <QNetworkReply>

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


struct JsonResponse {
    int statuscode;
    QJsonDocument data;
    QJsonParseError parseError;
    QString m_error;
    bool hasError();
    QString errorString();
};
Q_DECLARE_METATYPE(JsonResponse)

class JsonRequest : public QObject
{
    Q_OBJECT
public:
    static QSet<JsonRequest*>* pendingRequests;
    static void abortAllPendingRequest();
    explicit JsonRequest(QObject *parent = nullptr);
    virtual ~JsonRequest();
    RequestMethod method;
    QJsonDocument data;
    QString path;
    QString url;
    QString jwt;
    Q_INVOKABLE void send();
signals:
    void complete(JsonResponse resp);
protected:
    QNetworkAccessManager m_qnam;
    QNetworkReply* m_reply;
    void read_reply();
    void read_error();
    void do_done();
    RequestState m_state;
};

#endif // JSONREQUEST_H
