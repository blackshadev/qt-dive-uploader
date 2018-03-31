#ifndef FILEWRITER_H
#define FILEWRITER_H
#include "./jsondivewriter.h"

class FileDiveWriter : public JsonDiveWriter
{
public:
    FileDiveWriter(QString path);
    ~FileDiveWriter();
    virtual void do_start();
    virtual void do_end();

    void do_work(Dive* d) override;
    void set_device_info(uint model, uint serial, uint firmware) override;
    void set_device_descriptor(dc_descriptor_t *descr) override;

protected:
    t_computer computer;
    QFile file;
    QJsonArray jsonDives;
};

#endif // FILEWRITER_H
