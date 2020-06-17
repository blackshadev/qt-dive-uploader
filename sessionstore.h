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
    Q_PROPERTY(QString refreshToken READ getRefreshToken WRITE setRefreshToken NOTIFY refreshTokenChanged)
    Q_PROPERTY(QString writeType READ getWriteType WRITE setWriteType NOTIFY writeTypeChanged)
    Q_PROPERTY(QString transportType MEMBER m_transport_type NOTIFY transportTypeChanged)
    Q_PROPERTY(bool selectDives READ getSelectDives WRITE setSelectDives NOTIFY selectDivesChanged)
    Q_PROPERTY(bool useFingerprint READ getUseFingerprint WRITE setUseFingerprint NOTIFY useFingerprintChanged)



public:
    explicit SessionData(QObject* parent = NULL);

    QString getComputer();
    void setComputer(QString m_computer);

    QString getPath();
    void setPath(QString m_path);

    QString getRefreshToken();
    void setRefreshToken(QString m_refresh_token);

    QString getWriteType();
    void setWriteType(QString write_type);

    bool getSelectDives();
    void setSelectDives(bool selectDives);

    QString getTransportType();
    void setTransportType(QString trans);

    bool getUseFingerprint();
    void setUseFingerprint(bool useFingerprint);

    void read(const QJsonObject &json);
    void write(QJsonObject &json);

signals:
    void pathChanged(QString);
    void computerChanged(QString);
    void refreshTokenChanged(QString);
    void writeTypeChanged(QString);
    void selectDivesChanged(bool);
    void transportTypeChanged(QString);
    void useFingerprintChanged(bool);

protected :
    QString m_computer;
    QString m_path;
    QString m_refresh_token;
    QString m_write_type;
    QString m_transport_type;
    bool m_select_dives;
    bool m_use_fingerprint;
};

Q_DECLARE_METATYPE(SessionData*)

class SessionStore
{
public:
    SessionStore(QString path);
    ~SessionStore();
    bool save();
    bool load();
    SessionData m_data;
protected:
    QJsonDocument m_jsonDoc;
    QString m_path;

};

#endif // SESSIONSTORE_H
