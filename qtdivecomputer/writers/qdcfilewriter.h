#ifndef QDCFILEWRITER_H
#define QDCFILEWRITER_H

#include <QObject>
#include <QFile>
#include "qdcwriter.h"
#include "serializer/dcdiveserializer.h"
#include "serializer/dcdivecomputerserializer.h"

class QDCFileWriter : public QDCWriter
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
public:
    QDCFileWriter(QObject *parent = NULL);
    virtual ~QDCFileWriter();
    void setPath(QString p);
    QString getPath();

public slots:
    void write(DCDive *write) override;
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
Q_DECLARE_METATYPE(QDCFileWriter *)

#endif // QDCFILEWRITER_H
