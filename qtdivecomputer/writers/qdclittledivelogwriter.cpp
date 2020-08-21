#include "qdclittledivelogwriter.h"

QDCLittleDiveLogWriter::QDCLittleDiveLogWriter(QObject *parent)
    : QDCWriter(parent)
{}

QDCLittleDiveLogWriter::~QDCLittleDiveLogWriter()
{}

void QDCLittleDiveLogWriter::write(DCDive *dive)
{
    setBusy();

    QJsonObject json;
    diveSerializer.serialize(json, dive);
    json["computer_id"] = ldComputerId;

    divelog->request(
        RequestMethod::POST,
        "/dive",
        &json,
        [=](HTTPResponse *resp) {
            if(resp->hasError()) {
                emit error(resp->errorString());
            }

            unsetBusy();
            emit written();
        }
    );
}

void QDCLittleDiveLogWriter::end()
{
    emit ended();
}

void QDCLittleDiveLogWriter::cancel()
{
    emit cancelled();
}

void QDCLittleDiveLogWriter::start()
{
    setBusy();

    QJsonObject computer;
    computerSerializer.serialize(computer, device, descriptor);

    divelog->request(
        RequestMethod::POST,
        "/computer",
        &computer,
        [=](HTTPResponse *resp) {
            if(resp->hasError()) {
                emit error(resp->errorString());
                return;
            }

            auto obj = resp->getBodyAsJSON();
            ldComputerId = obj["computer_id"].toInt();

            unsetBusy();
            emit started();
        }
    );
}

LittleDiveLog *QDCLittleDiveLogWriter::getDiveLog()
{
    return divelog;
}

void QDCLittleDiveLogWriter::setDiveLog(LittleDiveLog *dl)
{
    divelog = dl;
}
