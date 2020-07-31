#ifndef JSONREQUEST_H
#define JSONREQUEST_H
#include <QObject>
#include <QJsonDocument>
#include <QtNetwork>
#include <QNetworkReply>

#undef DELETE

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
    static bool isStopped;
    static void abortAllPendingRequest();
    static void stop();
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
    static void cleanupStatics();
    QNetworkAccessManager m_qnam;
    QNetworkReply* m_reply;
    void read_reply();
    void read_error();
    void do_done();
    RequestState m_state;
};

#endif // JSONREQUEST_H
