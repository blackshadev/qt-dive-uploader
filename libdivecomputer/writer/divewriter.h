#ifndef DIVEWRITER_H
#define DIVEWRITER_H
#include "../dive.h"
#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

struct DiveWriter {
    virtual void begin() = 0;
    virtual void end() = 0;
    virtual void write(Dive* d) = 0;
};


class DiveWriterFile : public DiveWriter
{
public:
    DiveWriterFile(QString path);
    ~DiveWriterFile();
    void write(Dive* d) override;
    void begin() override;
    void end() override;
    static void write_tank(QJsonArray* tanksArray, Dive* dive);
    static void write_samples(QJsonArray* tanksArray, Dive* dive);

protected:
    QFile file;
    QJsonObject jsonObject;
    QJsonArray jsonDives;
};

#endif // DIVEWRITER_H
