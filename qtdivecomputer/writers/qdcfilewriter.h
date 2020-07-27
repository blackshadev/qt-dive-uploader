#ifndef QDCFILEWRITER_H
#define QDCFILEWRITER_H

#include <QObject>
#include <QFile>
#include "qdcwriter.h"
#include "serializer/dcdiveserializer.h"

class QDCFileWriter : public QDCWriter
{
    Q_OBJECT
    Q_PROPERTY(QString path READ getPath WRITE setPath NOTIFY pathChanged)
public:
    QDCFileWriter(QObject *parent = NULL);
    void setPath(QString p);
    QString getPath();
    void add(DCDive *dive) override;
    void end() override;
    void start() override;
private:
    DCDiveSerializer serializer;
    QString path;
    QJsonObject object;
    QJsonArray dives;
    QFile file;

signals:
    void pathChanged(QString path);
};

#endif // QDCFILEWRITER_H
