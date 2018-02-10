#include "sessionstore.h"

SessionData::SessionData(QObject *parent) : QObject(parent) {
    m_computer = "";
    m_path = "";
    m_portname = "";
}

void SessionData::setComputer(QString computer) {
    m_computer = computer;
    emit computerChanged(computer);
}
QString SessionData::getComputer() {
    return m_computer;
}

void SessionData::setPortname(QString portname) {
    m_portname = portname;
    emit portnameChanged(portname);
}
QString SessionData::getPortname() {
    return m_portname;
}


void SessionData::setPath(QString path) {
    m_path = path;
    emit pathChanged(path);
}

QString SessionData::getPath() {
    return m_path;
}

void SessionData::read(const QJsonObject &json)
{
    if(json.contains("path") && json["path"].isString()) {
        m_path = json["path"].toString();
    }

    if(json.contains("computer") && json["computer"].isString()) {
        m_computer = json["computer"].toString();
    }

    if(json.contains("portname") && json["portname"].isString()) {
        m_portname = json["portname"].toString();
    }

}

void SessionData::write(QJsonObject &json)
{
    json["path"] = m_path;
    json["computer"] = m_computer;
    json["portname"] = m_portname;
}

// -- SessionStore --
SessionStore::SessionStore(const char* path)
{
    m_path = QString(path);
}

SessionStore::~SessionStore()
{

}

bool SessionStore::load()
{
     QFile loadFile(m_path);

     if (!loadFile.open(QIODevice::ReadOnly)) {
         qWarning("Couldn't open save file.");
         return false;
     }

     QByteArray saveData = loadFile.readAll();
     QJsonDocument loadDoc(QJsonDocument::fromJson(saveData));
     m_data.read(loadDoc.object());

     return true;
}

bool SessionStore::save()
{
    QFile saveFile(m_path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject jsonObj;
    m_data.write(jsonObj);
    QJsonDocument saveDoc(jsonObj);
    saveFile.write(saveDoc.toJson());

    return true;
}
