#ifndef SELECTIONWRITER_H
#define SELECTIONWRITER_H
#include "divewriter.h"
#include "../dive.h"
#include "../qdivemodel.h"

class SelectionWriter : public DiveWriter
{
public:
    SelectionWriter(DiveWriter *writer);

public:
    void set_device_descriptor(dc_descriptor_t* descr) override;
    void set_device_info(uint model, uint serial, uint firmware) override;

    void do_start() override;
    void do_end() override;
    void do_work(Dive *d) override;
    QList<Dive> m_dives;
protected:
    DiveWriter* m_writer;
};

#endif // SELECTIONWRITER_H
