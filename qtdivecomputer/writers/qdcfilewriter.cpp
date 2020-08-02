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
    if (!getIsStarted()) {
        throw std::runtime_error("Not yet started");
    }

    QJsonObject diveObject;
    diveSerializer.serialize(diveObject, dive);
    dives.append(diveObject);

    writeCompleted();
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
    setIsStarted(false);
    setWriteReady(false);
    emit ended();
}

void QDCFileWriter::cancel()
{
    setIsStarted(false);
    setWriteReady(false);
    emit cancelled();
}

void QDCFileWriter::start()
{
    if (getIsStarted()) {
        throw std::runtime_error("Already started");
    }
    setIsStarted(true);

    dives = QJsonArray();
    object = QJsonObject();

    auto dt_now = datetime_now();
    object["readtime"] = QString::fromStdString(format_datetime_iso(dt_now));

    QJsonObject computerObject;
    computerSerializer.serialize(computerObject, device, descriptor);
    object["computer"] = computerObject;

    setWriteReady(true);
    emit started();
}

bool QDCFileWriter::isReady()
{
    return !path.isEmpty();
}




