#ifndef LITTLEDIVELOG_H
#define LITTLEDIVELOG_H

#include <QObject>
#include <QtNetwork>
#include "../jsonrequest.h"
#include <functional>
#include "./userinfo.h"

enum TokenType {
    NONE,
    REFRESH,
    ACCESS
};

class LittleDiveLog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant isLoggedIn READ isLoggedIn NOTIFY loggedStateChanged)
    Q_PROPERTY(UserInfo* userInfo MEMBER m_user_info NOTIFY userInfoChanged)
    Q_PROPERTY(QString refreshToken READ get_refresh_token NOTIFY refreshTokenChanged)

public:
    explicit LittleDiveLog(QObject *parent = nullptr);
    virtual ~LittleDiveLog();
    Q_INVOKABLE void login(QString username, QString password);
    Q_INVOKABLE void logout();
    bool isLoggedIn();
    QString get_refresh_token();
    void set_refresh_token(QString tok);
    void request(RequestMethod method, QString path, QJsonObject* data, std::function<void(JsonResponse)> callback, bool retry = true);
signals:
    void error(QString msg);
    void loggedStateChanged(bool isLoggedIn);
    void userInfoChanged(UserInfo* ui);
    void refreshTokenChanged(QString tok);
public slots:
protected:
    void get_user_data();
    void get_access_token(std::function<void()> callback);
    void raw_request(RequestMethod method, QString path, TokenType tokenType, QJsonObject* data, std::function<void(JsonResponse)> callback );
    QString m_refresh_token;
    QString m_access_token;
    UserInfo* m_user_info = NULL;
};

#endif // LITTLEDIVELOG_H
