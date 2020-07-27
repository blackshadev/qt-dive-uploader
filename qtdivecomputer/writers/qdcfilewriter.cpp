#include "qdcfilewriter.h"
#include "common/datetime.h"
#include <QJsonDocument>

QDCFileWriter::QDCFileWriter(QObject *parent)
    : QDCWriter(parent)
{}

void QDCFileWriter::setPath(QString p)
{
    path = p;
    file.setFileName(p);
    emit pathChanged(p);
}

QString QDCFileWriter::getPath()
{
    return path;
}

void QDCFileWriter::add(DCDive *dive)
{
    QJsonObject diveObject;
    serializer.serialize(diveObject, dive);
    dives.append(diveObject);
}

void QDCFileWriter::end()
{
    object["dives"] = dives;

    QJsonDocument document(object);
    file.write(document.toJson());
    file.close();

}

void QDCFileWriter::start()
{
    if(file.isOpen()) {
        throw std::runtime_error("File already open");
    }

    dives.empty();
    auto dt_now = datetime_now();
    object["readtime"] = QString::fromStdString(format_datetime_iso(dt_now));

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        auto error = file.error();
        QString errorText = "Couldn't open save file. ";
        errorText.append(error);
        throw std::runtime_error(errorText.toStdString());
    }
}


