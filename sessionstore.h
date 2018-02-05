#ifndef SESSIONSTORE_H
#define SESSIONSTORE_H
#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <iostream>
#include <fstream>
using namespace std;

class SessionData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString m_path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString m_computer READ getComputer WRITE setComputer NOTIFY computerChanged)

public:

    QString getComputer();
    void setComputer(QString m_computer);

    QString getPath();
    void setPath(QString m_path);

    void read(const QJsonObject &json);
    void write(QJsonObject &json);

signals:
    void pathChanged(QString);
    void computerChanged(QString);

protected :
    QString m_computer;
    QString m_path;

};


Q_DECLARE_METATYPE(SessionData*)

class SessionStore
{
public:
    SessionStore(const char* path);
    ~SessionStore();
    bool save();
    bool load();
    SessionData data;
protected:
    QJsonDocument jsonDoc;
    QString path;

};

#endif // SESSIONSTORE_H
