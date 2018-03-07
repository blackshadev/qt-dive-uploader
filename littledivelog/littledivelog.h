#ifndef LITTLEDIVELOG_H
#define LITTLEDIVELOG_H

#include <QObject>
#include <QtNetwork>
#include "../jsonrequest.h"
#include <functional>

enum TokenType {
    NONE,
    REFRESH,
    ACCESS
};

class LittleDiveLog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant isLoggedIn READ isLoggedIn CONSTANT)
public:
    explicit LittleDiveLog(QObject *parent = nullptr);
    Q_INVOKABLE void login(QString username, QString password);
    Q_INVOKABLE void logout();
    bool isLoggedIn();
signals:
    void error(QString msg);
    void loggedIn();
    void loggedOut();
public slots:
protected:
    void get_access_token(std::function<void()> callback);
    void request(RequestMethod method, QString path, QJsonObject* data, std::function<void(JsonResponse)> callback );
    void raw_request(RequestMethod method, QString path, TokenType tokenType, QJsonObject* data, std::function<void(JsonResponse)> callback );
    QString m_refresh_token;
    QString m_access_token;
};

#endif // LITTLEDIVELOG_H
