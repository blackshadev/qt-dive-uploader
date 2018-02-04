#include "sessionstore.h"

void SessionData::setComputer(QString computer) {
    this->computer = computer;
    emit computerChanged(computer);
}
QString SessionData::getComputer() {
    return computer;
}

void SessionData::setPath(QString path) {
    this->path = path;
    emit pathChanged(path);
}

QString SessionData::getPath() {
    return path;
}

void SessionData::read(const QJsonObject &json)
{
    if(json.contains("path") && json["path"].isString()) {
        path = json["path"].toString();
    }

    if(json.contains("computer") && json["computer"].isString()) {
        computer = json["computer"].toString();
    }

}

void SessionData::write(QJsonObject &json)
{
    json["path"] = path;
    json["computer"] = computer;
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
