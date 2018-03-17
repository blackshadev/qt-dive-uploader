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
    Q_PROPERTY(QString refreshToken READ getRefreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged)
    Q_PROPERTY(QString writeType READ getWriteType WRITE setWriteType NOTIFY writeTypeChanged)


public:
    explicit SessionData(QObject* parent = NULL);

    QString getComputer();
    void setComputer(QString m_computer);

    QString getPath();
    void setPath(QString m_path);

    QString getPortname();
    void setPortname(QString m_portname);

    QString getRefreshToken();
    void setRefreshToken(QString m_refresh_token);

    QString getWriteType();
    void setWriteType(QString write_type);


    void read(const QJsonObject &json);
    void write(QJsonObject &json);

signals:
    void pathChanged(QString);
    void computerChanged(QString);
    void portnameChanged(QString);
    void refreshTokenChanged(QString);
    void writeTypeChanged(QString);

protected :
    QString m_computer;
    QString m_path;
    QString m_portname;
    QString m_refresh_token;
    QString m_write_type;
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
