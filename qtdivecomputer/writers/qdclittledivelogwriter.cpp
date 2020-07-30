#include "qdclittledivelogwriter.h"

QDCLittleDiveLogWriter::QDCLittleDiveLogWriter(QObject *parent)
    : QDCWriter(parent)
{

}

void QDCLittleDiveLogWriter::write(DCDive *dive)
{

}

void QDCLittleDiveLogWriter::end()
{

}

void QDCLittleDiveLogWriter::cancel()
{

}

void QDCLittleDiveLogWriter::start()
{
    QJsonObject computer;
    computerSerializer.serialize(computer, device, descriptor);

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

            readyForWrites();
        }
    );
}

bool QDCLittleDiveLogWriter::isReady()
{
    return divelog != NULL && divelog->isLoggedIn() && divelog->hasUserData();
}

LittleDiveLog *QDCLittleDiveLogWriter::getLittleDiveLog()
{
    return divelog;
}

void QDCLittleDiveLogWriter::setLittleDiveLog(LittleDiveLog *dl)
{
    divelog = dl;
    connect(dl, SIGNAL(userInfoChanged(UserInfo *)), this, SIGNAL(isReadyChanged()));
    emit isReadyChanged();
}
