#ifndef QDCFILEEXPORTER_H
#define QDCFILEEXPORTER_H

#include <QObject>
#include <QFile>
#include "qdcwritetarget.h"
#include "serializer/dcdiveserializer.h"
#include "serializer/dcdivecomputerserializer.h"

class QDCFileExporter : public QDCWriteTarget
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
public:
    QDCFileExporter(QObject *parent = NULL);
    void setPath(QString p);
    QString getPath();

public slots:
    void write(QDCDive *write) override;
    void end() override;
    void cancel() override;
    void start() override;

private:
    QJsonObject getComputerAsJson();
    DCDiveSerializer diveSerializer;
    DCDiveComputerSerializer computerSerializer;
    QString path;
    QJsonObject object;
    QJsonArray dives;

signals:
    void pathChanged(QString path);
};
Q_DECLARE_METATYPE(QDCFileExporter *)

#endif // QDCFILEEXPORTER_H
