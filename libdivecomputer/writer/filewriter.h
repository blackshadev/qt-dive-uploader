#ifndef FILEWRITER_H
#define FILEWRITER_H
#include "./divewriter.h"


class FileDiveWriter : public DiveWriter
{public:
    FileDiveWriter(QString path);
    ~FileDiveWriter();
    void begin();
    void end();
    virtual void write(Dive* d);

    void set_device_info(uint model, uint serial, uint firmware) override;
    void set_device_descriptor(dc_descriptor_t *descr) override;

    static void write_dive(QJsonObject &target, Dive* dive);
    static void write_tank(QJsonArray &tanksArray, Dive* dive);
    static void write_samples(QJsonArray &tanksArray, Dive* dive);

protected:
    QFile file;
    QJsonArray jsonDives;
    QJsonObject jsonComputer;
};

#endif // FILEWRITER_H
