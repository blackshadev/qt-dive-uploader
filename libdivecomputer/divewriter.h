#ifndef DIVEWRITER_H
#define DIVEWRITER_H
#include "dive.h"#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>


class DiveWriter
{
public:
    DiveWriter();
    ~DiveWriter();
    void write(Dive* d);
    void close();
    void open();
    void setFileName(QString path);
protected:
    QFile file;
    QJsonObject jsonObject;
    QJsonArray jsonDives;
    void end();
};

#endif // DIVEWRITER_H
