#ifndef LITTLEDIVELOG_H
#define LITTLEDIVELOG_H

#include <QObject>
#include <QtNetwork>
#include "../http/jsonrequest.h"
#include "../http/requestcontainer.h"
#include <functional>
#include "./userinfo.h"
#include "littledivelogcomputer.h"

enum RequestTokenType {
    NONE,
    REFRESH,
    ACCESS
};

class LittleDiveLog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant isLoggedIn READ isLoggedIn NOTIFY loggedStateChanged)
    Q_PROPERTY(UserInfo* userInfo MEMBER m_user_info NOTIFY userInfoChanged)
    Q_PROPERTY(QString refreshToken WRITE set_refresh_token READ get_refresh_token NOTIFY refreshTokenChanged)
    Q_PROPERTY(RequestContainer *requestContainer WRITE setRequestContainer)

public:
    explicit LittleDiveLog(QObject *parent = nullptr);
    virtual ~LittleDiveLog();
    Q_INVOKABLE void login(QString username, QString password);
    Q_INVOKABLE void logout();
    bool isLoggedIn();
    QString get_refresh_token();
    void set_refresh_token(QString tok);
    void request(RequestMethod method, QString path, QJsonObject* data, std::function<void(HTTPResponse *)> callback, bool retry = true, QObject* parent = NULL);
    UserInfo* m_user_info = NULL;
    void fetch_user_data();
    bool hasUserData();
signals:
    void error(QString msg);
    void loggedStateChanged(bool isLoggedIn);
    void userInfoChanged(UserInfo* ui);
    void refreshTokenChanged(QString tok);
public slots:
    QVariant getComputer(QDeviceData data);
    void setRequestContainer(RequestContainer *req);
protected:
    void fetch_user_computers(std::function<void()> callback);
    void get_access_token(std::function<void()> callback, QObject* parent = NULL);
    void raw_request(RequestMethod method, QString path, RequestTokenType tokenType, QJsonObject* data, std::function<void(HTTPResponse *)> callback, QObject* parent = NULL);
    QString m_refresh_token;
    QString m_access_token;
    RequestContainer *requests;
};

#endif // LITTLEDIVELOG_H
