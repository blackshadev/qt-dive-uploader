#include "qdcfilewriter.h"
#include "common/datetime.h"

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

void QDCFileWriter::add(DCDive *dive)
{
}

void QDCFileWriter::end()
{

}

void QDCFileWriter::start()
{
    QJsonArray arr;
    object["dives"] = arr;
    auto dt_now = datetime_now();
    object["readtime"] = QString::fromStdString(format_datetime_iso(dt_now));


}


