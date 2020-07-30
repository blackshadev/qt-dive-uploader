#include "qdcfilewriter.h"
#include "../divecomputer/utils/datetime.h"
#include <QJsonDocument>

QDCFileWriter::QDCFileWriter(QObject *parent)
    : QDCWriter(parent)
{}

void QDCFileWriter::setPath(QString p)
{
    path = p;
    emit pathChanged(p);
    emit isReadyChanged();
}

QString QDCFileWriter::getPath()
{
    return path;
}

void QDCFileWriter::write(DCDive *dive)
{
    if (!isBusy) {
        throw std::runtime_error("Not yet started");
    }

    QJsonObject diveObject;
    diveSerializer.serialize(diveObject, dive);
    dives.append(diveObject);
}

void QDCFileWriter::end()
{
    object["dives"] = dives;

    QFile file;
    file.setFileName(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        auto error = file.error();
        QString errorText = "Couldn't open save file. ";
        errorText.append(error);
        throw std::runtime_error(errorText.toStdString());
    }

    QJsonDocument document(object);
    file.write(document.toJson());
    file.close();
    setIsBusy(false);
}

void QDCFileWriter::cancel()
{
    setIsBusy(false);
}

void QDCFileWriter::start()
{
    if (isBusy) {
        throw std::runtime_error("Already started");
    }
    setIsBusy(true);

    dives = QJsonArray();
    object = QJsonObject();

    auto dt_now = datetime_now();
    object["readtime"] = QString::fromStdString(format_datetime_iso(dt_now));

    QJsonObject computerObject;
    computerSerializer.serialize(computerObject, device, descriptor);
    object["computer"] = computerObject;

    readyForWrites();
}

bool QDCFileWriter::isReady()
{
    return !path.isEmpty();
}



