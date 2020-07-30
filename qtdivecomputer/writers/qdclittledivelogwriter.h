#ifndef QDCLITTLELOGWRITER_H
#define QDCLITTLELOGWRITER_H

#include <QObject>
#include <QObject>
#include <QFile>
#include "qdcwriter.h"
#include "serializer/dcdiveserializer.h"
#include "serializer/dcdivecomputerserializer.h"
#include "littledivelog/littledivelog.h"

class QDCLittleDiveLogWriter : public QDCWriter
{
    Q_OBJECT
    Q_PROPERTY(LittleDiveLog *littledivelog READ getLittleDiveLog WRITE setLittleDiveLog)

public:
    QDCLittleDiveLogWriter(QObject *parent = NULL);
    void write(DCDive *dive) override;
    void end() override;
    void cancel() override;
    void start() override;
    bool isReady() override;
    LittleDiveLog *getLittleDiveLog();
    void setLittleDiveLog(LittleDiveLog *dl);
private:
    DCDiveSerializer diveSerializer;
    DCDiveComputerSerializer computerSerializer;
    LittleDiveLog *divelog = NULL;
    int ldComputerId;
signals:

};
Q_DECLARE_METATYPE(QDCLittleDiveLogWriter *)

#endif // QDCLITTLELOGWRITER_H
