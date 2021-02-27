#include "qdclittledivelogexporter.h"

QDCLittleDiveLogExporter::QDCLittleDiveLogExporter(QObject *parent)
    : QDCWriteTarget(parent)
{}


void QDCLittleDiveLogExporter::write(QDCDive *dive)
{
    setBusy();

    QJsonObject json;
    diveSerializer.serialize(json, dive);
    json["computer_id"] = ldComputerId;

    divelog->request(
        RequestMethod::POST,
        "/dives",
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

void QDCLittleDiveLogExporter::end()
{
    emit ended();
}

void QDCLittleDiveLogExporter::cancel()
{
    emit cancelled();
}

void QDCLittleDiveLogExporter::start()
{
    setBusy();

    QJsonObject computer;
    computerSerializer.serialize(computer, getDevice(), getDescriptor());

    divelog->request(
        RequestMethod::POST,
        "/computers",
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

LittleDiveLog *QDCLittleDiveLogExporter::getDiveLog()
{
    return divelog;
}

void QDCLittleDiveLogExporter::setDiveLog(LittleDiveLog *dl)
{
    divelog = dl;
}
