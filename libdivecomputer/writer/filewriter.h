#ifndef FILEWRITER_H
#define FILEWRITER_H
#include "./jsondivewriter.h"

struct t_computer {
    dc_descriptor_t* descr;
    uint serial;
    uint firmware;
};

class FileDiveWriter : public JsonDiveWriter
{
public:
    FileDiveWriter(QString path);
    ~FileDiveWriter();
    void begin();
    void end();
    virtual void write(Dive* d);

    void set_device_info(uint model, uint serial, uint firmware) override;
    void set_device_descriptor(dc_descriptor_t *descr) override;

protected:
    t_computer computer;
    QFile file;
    QJsonArray jsonDives;
};

#endif // FILEWRITER_H
