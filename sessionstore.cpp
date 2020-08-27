#include "sessionstore.h"
#include <QDir>

SessionData::SessionData(QObject *parent) : QObject(parent) {
    m_computer = "";
    m_path = "";
    m_select_dives = false;
    m_use_fingerprint = true;
}

void SessionData::setComputer(QString computer) {
    m_computer = computer;
    emit computerChanged(computer);
}
QString SessionData::getComputer() {
    return m_computer;
}

QString SessionData::getWriteType()
{
    return m_write_type;
}

void SessionData::setWriteType(QString write_type)
{
    m_write_type = write_type;
    emit writeTypeChanged(write_type);
}

bool SessionData::getSelectDives()
{
    return m_select_dives;
}

void SessionData::setSelectDives(bool selectDives)
{
    m_select_dives = selectDives;
    emit selectDivesChanged(selectDives);
}

void SessionData::setUseFingerprint(bool useFingerprint)
{
    m_use_fingerprint = useFingerprint;
    emit useFingerprintChanged(useFingerprint);
}

bool SessionData::getUseFingerprint()
{
    return m_use_fingerprint;
}

void SessionData::setPath(QString path) {
    m_path = path;
    emit pathChanged(path);
}

QString SessionData::getPath() {
    return m_path;
}

QString SessionData::getRefreshToken() {
    return m_refresh_token;
}

void SessionData::setRefreshToken(QString tok) {
    m_refresh_token = tok;
    emit refreshTokenChanged(tok);
}

void SessionData::setTransportType(QString trans) {
    m_transport_type = trans;
    emit transportTypeChanged(trans);
}

QString SessionData::getTransportType() {
    return m_transport_type;
}

void SessionData::read(const QJsonObject &json)
{
    if(json.contains("path") && json["path"].isString()) {
        setPath(json["path"].toString());
    }

    if(json.contains("computer") && json["computer"].isString()) {
        setComputer(json["computer"].toString());
    }

    if(json.contains("refreshToken") && json["refreshToken"].isString()) {
        setRefreshToken(json["refreshToken"].toString());
    }

    if(json.contains("writeType") && json["writeType"].isString()) {
        setWriteType(json["writeType"].toString());
    }

    if(json.contains("selectDives") && json["selectDives"].isBool()) {
        setSelectDives(json["selectDives"].toBool());
    }

    if(json.contains("transportType") && json["transportType"].isString()) {
        setTransportType(json["transportType"].toString());
    }

    if(json.contains("useFingerprint") && json["useFingerprint"].isString()) {
        setUseFingerprint(json["useFingerprint"].toBool());
    }
}

void SessionData::write(QJsonObject &json)
{
    json["path"] = m_path;
    json["computer"] = m_computer;
    json["refreshToken"] = m_refresh_token;
    json["writeType"] = m_write_type;
    json["selectDives"] = m_select_dives;
    json["transportType"] = m_transport_type;
    json["useFingerprint"] = m_use_fingerprint;
}

// -- SessionStore --
SessionStore::SessionStore() : QObject()
{
    connect(&sessionData, SIGNAL(pathChanged(QString)), this, SLOT(save()));
    connect(&sessionData, SIGNAL(computerChanged(QString)), this, SLOT(save()));
    connect(&sessionData, SIGNAL(refreshTokenChanged(QString)), this, SLOT(save()));
    connect(&sessionData, SIGNAL(writeTypeChanged(QString)), this, SLOT(save()));
    connect(&sessionData, SIGNAL(selectDivesChanged(bool)), this, SLOT(save()));
    connect(&sessionData, SIGNAL(transportTypeChanged(QString)), this, SLOT(save()));
    connect(&sessionData, SIGNAL(useFingerprintChanged(bool)), this, SLOT(save()));

}

SessionStore::~SessionStore()
{

}

void SessionStore::setPath(QString p)
{
    path = p;
    isLoaded = false;
    emit pathChanged(p);
}

QString SessionStore::getPath()
{
    return path;
}

SessionData *SessionStore::getData()
{
    return &sessionData;
}

bool SessionStore::load()
{
    QFile loadFile(path);
    if (!loadFile.open(QIODevice::ReadOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QByteArray saveData = loadFile.readAll();
    QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
    sessionData.read(loadDoc.object());

    isLoaded = true;
    emit loaded();
    return true;
}

bool SessionStore::save()
{
    if (!isLoaded) {
        return false;
    }

    QFileInfo fileInfo(path);
    fileInfo.absoluteDir().mkpath(".");
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject jsonObj;
    sessionData.write(jsonObj);
    QJsonDocument saveDoc(jsonObj);
    saveFile.write(saveDoc.toJson());

    return true;
}
