#include "userinfo.h"
#include <QJsonObject>

UserInfo::UserInfo(QObject *parent) : QObject(parent)
{
}

UserInfo::~UserInfo()
{
    for(std::pair<uint, LittleDiveLogComputer *> comp : m_computers) {
        delete comp.second;
    }

}

void UserInfo::addComputer(QJsonObject obj)
{
    auto comp = LittleDiveLogComputer::fromData(obj);

    m_computers[comp->getSerial()] = comp;
}

LittleDiveLogComputer *UserInfo::getComputer(QDeviceData data)
{
    auto computer = m_computers.find(data.serial);
    return computer == m_computers.end() ? NULL : computer->second;
}
