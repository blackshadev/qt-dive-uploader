#ifndef SELECTIONWRITER_H
#define SELECTIONWRITER_H
#include "divewriter.h"
#include "../dive.h"
#include "../qdivemodel.h"

class SelectionWriter : public DiveWriter
{
public:
    SelectionWriter(DiveWriter *writer);
    ~SelectionWriter();
    void set_device_descriptor(dc_descriptor_t* descr) override;
    void set_device_info(uint model, uint serial, uint firmware) override;
    void set_device_clock(uint devtime, uint systime) override;

    void do_start() override;
    void do_end() override;
    void do_work(Dive *d) override;
    QList<Dive> m_dives;
public slots:
    void selected(QList<Dive> dives);
signals:
    void select(QList<Dive> dives);
protected:
    DiveWriter* m_writer;

    dc_descriptor_t* m_descriptor;

    uint m_model;
    uint m_serial;
    uint m_firmware;

    uint m_devtime;
    uint m_systime;
};

#endif // SELECTIONWRITER_H
