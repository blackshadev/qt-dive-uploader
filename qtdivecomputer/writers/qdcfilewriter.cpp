#include "qdcfilewriter.h"
#include "common/datetime.h"
#include <QJsonDocument>

QDCFileWriter::QDCFileWriter(QObject *parent)
    : QDCWriter(parent)
{}

void QDCFileWriter::setPath(QString p)
{
    path = p;
    emit pathChanged(p);
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
    serializer.serialize(diveObject, dive);
    dives.append(diveObject);
}

void QDCFileWriter::end()
{
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
    setBusy(false);
}

void QDCFileWriter::cancel()
{
    setBusy(false);
}

void QDCFileWriter::start()
{
    if (isBusy) {
        throw std::runtime_error("Already started");
    }
    setBusy(true);

    dives = QJsonArray();
    object = QJsonObject();

    auto dt_now = datetime_now();
    object["readtime"] = QString::fromStdString(format_datetime_iso(dt_now));
    object["computer"] = getComputerAsJson();
    object["dives"] = dives;
}

QJsonObject QDCFileWriter::getComputerAsJson()
{
    QJsonObject json;
    json["serial"] =  (int)(device.serial);
    json["vendor"] = descriptor->getQVendor();
    json["model"] = (int)(descriptor->getModelNumber());
    json["type"] = (int)(descriptor->getFamilyType());
    json["name"] = descriptor->getQDescription();

    return json;
}


