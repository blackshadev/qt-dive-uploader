#include "userinfo.h"
#include <QJsonObject>
#include "../common/base64.h"

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
    qInfo("here");
    auto comp = new t_user_computer;
    int comp_id = obj["computer_id"].toInt();
    uint serial = (uint)std::stoul(obj["serial"].toString().toStdString());
    auto fingerprint = base64_decode(obj["last_fingerprint"].toString());
    comp->id = comp_id;
    comp->fingerprint = fingerprint;

    m_computers[serial] = comp;
}

t_user_computer* UserInfo::get_computer(uint serial)
{
    return m_computers[serial];
}
