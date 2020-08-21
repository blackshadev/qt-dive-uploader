#include "qdcfilewriter.h"
#include "../divecomputer/utils/datetime.h"
#include <QJsonDocument>

QDCFileWriter::QDCFileWriter(QObject *parent)
    : QDCWriter(parent)
{}

QDCFileWriter::~QDCFileWriter()
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
    setBusy();
    QJsonObject diveObject;
    diveSerializer.serialize(diveObject, dive);
    dives.append(diveObject);
    unsetBusy();
}

void QDCFileWriter::end()
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

void QDCFileWriter::cancel()
{
    emit cancelled();
}

void QDCFileWriter::start()
{
    unsetBusy();

    dives = QJsonArray();
    object = QJsonObject();

    auto dt_now = datetime_now();
    object["readtime"] = QString::fromStdString(format_datetime_iso(dt_now));

    QJsonObject computerObject;
    computerSerializer.serialize(computerObject, device, descriptor);
    object["computer"] = computerObject;

    emit started();

    setBusy();
}




