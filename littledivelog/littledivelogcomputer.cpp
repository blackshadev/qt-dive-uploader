#include "littledivelogcomputer.h"


LittleDiveLogComputer *LittleDiveLogComputer::fromData(QJsonObject obj, QObject *parent)
{
    auto comp = new LittleDiveLogComputer(parent);

    int comp_id = obj["computer_id"].toInt();
    uint serial = (uint)std::stoul(obj["serial"].toString().toStdString());
    auto strFingerprint = obj["last_fingerprint"].toString();
    auto fingerprint = QByteArray::fromBase64(strFingerprint.toLatin1());

    comp->id = comp_id;
    comp->serial = serial;
    comp->fingerprint = fingerprint;

    return comp;
}

LittleDiveLogComputer::LittleDiveLogComputer(QObject *parent)
    : QObject(parent)
{}

QByteArray LittleDiveLogComputer::getFingerprint()
{
    return fingerprint;
}

int LittleDiveLogComputer::getId()
{
    return id;
}

unsigned int LittleDiveLogComputer::getSerial()
{
    return serial;
}
