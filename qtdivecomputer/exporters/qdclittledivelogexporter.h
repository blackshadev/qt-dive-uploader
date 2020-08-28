#ifndef QDCLITTLELOGWRITER_H
#define QDCLITTLELOGWRITER_H

#include <QObject>
#include <QObject>
#include <QFile>
#include "qdcwritetarget.h"
#include "serializer/dcdiveserializer.h"
#include "serializer/dcdivecomputerserializer.h"
#include "littledivelog/littledivelog.h"

class QDCLittleDiveLogExporter : public QDCWriteTarget
{
    Q_OBJECT
    Q_PROPERTY(LittleDiveLog *divelog READ getDiveLog WRITE setDiveLog)

public:
    QDCLittleDiveLogExporter(QObject *parent = NULL);
    LittleDiveLog *getDiveLog();
    void setDiveLog(LittleDiveLog *dl);
    void setWriting();

public slots:
    void write(QDCDive *write) override;
    void end() override;
    void cancel() override;
    void start() override;

private:
    DCDiveSerializer diveSerializer;
    DCDiveComputerSerializer computerSerializer;
    LittleDiveLog *divelog = NULL;
    int ldComputerId;
    bool isWriting = false;

signals:
    void isWriteReadyChanged();

};
Q_DECLARE_METATYPE(QDCLittleDiveLogExporter *)

#endif // QDCLITTLELOGWRITER_H
