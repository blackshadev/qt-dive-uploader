#include "qdclittledivelogwriter.h"

QDCLittleDiveLogWriter::QDCLittleDiveLogWriter(QObject *parent)
    : QDCWriter(parent)
{

}

void QDCLittleDiveLogWriter::write(DCDive *dive)
{
    if (!getIsBusy()) {
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
        [=](JsonResponse resp) {
            if(resp.hasError()) {
                emit error(resp.errorString());
            }

            writeCompleted();
        }
    );
}

void QDCLittleDiveLogWriter::end()
{

}

void QDCLittleDiveLogWriter::cancel()
{
    setIsBusy(false);
}

void QDCLittleDiveLogWriter::start()
{
    if (getIsBusy()) {
        throw std::runtime_error("Already started");
    }

    QJsonObject computer;
    computerSerializer.serialize(computer, device, descriptor);

    setWriteReady(false);
    divelog->request(
        RequestMethod::POST,
        "/computer",
        &computer,
        [=](JsonResponse resp) {
            if(resp.hasError()) {
                emit error(resp.errorString());
                return;
            }
            auto obj = resp.data.object();
            ldComputerId = obj["computer_id"].toInt();

            setWriteReady(true);
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
