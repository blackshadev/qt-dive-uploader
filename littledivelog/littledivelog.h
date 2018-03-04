#ifndef LITTLEDIVELOG_H
#define LITTLEDIVELOG_H

#include <QObject>
#include <QtNetwork>

class LittleDiveLog : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QVariant isLoggedIn READ isLoggedIn CONSTANT)
public:
    explicit LittleDiveLog(QObject *parent = nullptr);
    Q_INVOKABLE void login(QString username, QString password);
    bool isLoggedIn();
signals:
    void error(QString msg);
    void loggedIn();
    void loggedOut();
public slots:
protected:
    QString m_token;
};

#endif // LITTLEDIVELOG_H
