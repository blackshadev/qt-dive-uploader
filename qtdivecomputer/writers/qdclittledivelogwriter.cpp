#include "qdclittledivelogwriter.h"

QDCLittleDiveLogWriter::QDCLittleDiveLogWriter(QObject *parent)
    : QDCWriter(parent)
{

}

void QDCLittleDiveLogWriter::write(DCDive *dive)
{
    if (!getIsStarted()) {
        throw std::runtime_error("Not yet started");
    }

    QJsonObject json;
    diveSerializer.serialize(json, dive);
    json["computer_id"] = ldComputerId;

    setWriteReady(false);
    divelog->request(
        RequestMethod::POST,
        "/dive",
        &json,
        [=](HTTPResponse *resp) {
            if(resp->hasError()) {
                emit error(resp->errorString());
            }

            writeCompleted();
        }
    );
}

void QDCLittleDiveLogWriter::end()
{
    setWriteReady(false);
    emit ended();
}

void QDCLittleDiveLogWriter::cancel()
{
    setIsStarted(false);
    setWriteReady(false);
    emit cancelled();
}

void QDCLittleDiveLogWriter::start()
{
    if (getIsStarted()) {
        throw std::runtime_error("Already started");
    }

    QJsonObject computer;
    computerSerializer.serialize(computer, device, descriptor);

    setWriteReady(false);
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

            setWriteReady(true);
            emit started();
        }
    );
}

bool QDCLittleDiveLogWriter::isReady()
{
    return divelog != NULL && divelog->isLoggedIn() && divelog->hasUserData();
}

LittleDiveLog *QDCLittleDiveLogWriter::getDiveLog()
{
    return divelog;
}

void QDCLittleDiveLogWriter::setDiveLog(LittleDiveLog *dl)
{
    divelog = dl;
    connect(dl, SIGNAL(userInfoChanged(UserInfo *)), this, SIGNAL(isReadyChanged()));
    emit isReadyChanged();
}
