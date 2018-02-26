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
    void set_filename(QString path);
    static void write_tank(QJsonArray* tanksArray, Dive* dive);
    static void write_samples(QJsonArray* tanksArray, Dive* dive);

protected:
    QFile file;
    QJsonObject jsonObject;
    QJsonArray jsonDives;
    void end();
};

#endif // DIVEWRITER_H
