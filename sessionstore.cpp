#include "sessionstore.h"

void SessionData::setComputer(QString computer) {
    this->m_computer = computer;
    emit computerChanged(computer);
}
QString SessionData::getComputer() {
    return m_computer;
}

void SessionData::setPath(QString path) {
    this->m_path = path;
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

}

void SessionData::write(QJsonObject &json)
{
    json["path"] = m_path;
    json["computer"] = m_computer;
}

SessionStore::SessionStore(const char* path)
{
    this->path = QString(path);
}

SessionStore::~SessionStore()
{

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
     data.read(loadDoc.object());

     return true;
}

bool SessionStore::save()
{
    QFile saveFile(path);

    if (!saveFile.open(QIODevice::WriteOnly)) {
        qWarning("Couldn't open save file.");
        return false;
    }

    QJsonObject jsonObj;
    data.write(jsonObj);
    QJsonDocument saveDoc(jsonObj);
    saveFile.write(saveDoc.toJson());

    return true;
}
