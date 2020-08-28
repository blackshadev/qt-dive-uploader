#include "qdcfileexporter.h"
#include "../divecomputer/utils/datetime.h"
#include <QJsonDocument>

QDCFileExporter::QDCFileExporter(QObject *parent)
    : QDCWriteTarget(parent)
{}

void QDCFileExporter::setPath(QString p)
{
    path = p;
    emit pathChanged(p);
}

QString QDCFileExporter::getPath()
{
    return path;
}

void QDCFileExporter::write(QDCDive *dive)
{
    setBusy();
    QJsonObject diveObject;
    diveSerializer.serialize(diveObject, dive);
    dives.append(diveObject);
    unsetBusy();
    emit written();
}

void QDCFileExporter::end()
{
    object["dives"] = dives;

    QFile file;
    file.setFileName(path);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        auto fileError = file.error();
        QString errorText = "Couldn't open save file. ";
        errorText.append(fileError);
        emit error(errorText);
        return;
    }

    QJsonDocument document(object);
    file.write(document.toJson());
    file.close();
    emit ended();
}

void QDCFileExporter::cancel()
{
    emit cancelled();
}

void QDCFileExporter::start()
{
    setBusy();

    dives = QJsonArray();
    object = QJsonObject();

    auto dt_now = datetime_now();
    object["readtime"] = QString::fromStdString(format_datetime_iso(dt_now));

    QJsonObject computerObject;
    computerSerializer.serialize(computerObject, getDevice(), getDescriptor());
    object["computer"] = computerObject;

    unsetBusy();
    emit started();

}




