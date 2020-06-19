#include "userinfo.h"
#include <QJsonObject>

UserInfo::UserInfo(QObject *parent) : QObject(parent)
{

}

UserInfo::~UserInfo()
{
    for(std::pair<uint, t_user_computer*> comp : m_computers) {
        delete comp.second;
    }

}

void UserInfo::add_computer(QJsonObject obj)
{
    auto comp = new t_user_computer;
    int comp_id = obj["computer_id"].toInt();
    uint serial = (uint)std::stoul(obj["serial"].toString().toStdString());
    auto strFingerprint = obj["last_fingerprint"].toString();
    auto fingerprint = QByteArray::fromBase64(strFingerprint.toLatin1());
    comp->id = comp_id;
    comp->fingerprint = fingerprint;

    m_computers[serial] = comp;
}

t_user_computer* UserInfo::get_computer(uint serial)
{
    auto data = m_computers.find(serial);
    return data == m_computers.end() ? NULL : data->second;
}
