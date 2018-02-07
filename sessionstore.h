#ifndef SESSIONSTORE_H
#define SESSIONSTORE_H
#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
using namespace std;

class SessionData : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
    Q_PROPERTY(QString computer READ getComputer WRITE setComputer NOTIFY computerChanged)
    Q_PROPERTY(QString portname READ getPortname WRITE setPortname NOTIFY portnameChanged)

public:
    explicit SessionData(QObject* parent = NULL);

    QString getComputer();
    void setComputer(QString m_computer);

    QString getPath();
    void setPath(QString m_path);

    QString getPortname();
    void setPortname(QString m_portname);

    void read(const QJsonObject &json);
    void write(QJsonObject &json);

signals:
    void pathChanged(QString);
    void computerChanged(QString);
    void portnameChanged(QString);

protected :
    QString m_computer;
    QString m_path;
    QString m_portname;

};


Q_DECLARE_METATYPE(SessionData*)

class SessionStore
{
public:
    SessionStore(const char* path);
    ~SessionStore();
    bool save();
    bool load();
    SessionData m_data;
protected:
    QJsonDocument m_jsonDoc;
    QString m_path;

};

#endif // SESSIONSTORE_H
