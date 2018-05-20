#include "selectionwriter.h"

SelectionWriter::SelectionWriter(DiveWriter *writer)
{
    m_writer = writer;

    connect(m_writer, &DiveWriter::diveWritten, [this](Dive* d) {
        emit diveWritten(d);
    });
}

SelectionWriter::~SelectionWriter()
{
    delete m_writer;
    m_writer = NULL;
}

void SelectionWriter::selected(QList<Dive> dives)
{
    m_writer->start();
}

void SelectionWriter::do_start()
{

}

void SelectionWriter::do_end()
{
    emit select(m_dives);
}

void SelectionWriter::do_work(Dive *d)
{
    m_dives.append(*d);
}

void SelectionWriter::set_device_descriptor(dc_descriptor_t *descr)
{
    m_descriptor = descr;
}

void SelectionWriter::set_device_info(uint model, uint serial, uint firmware)
{
    m_model = model;
    m_serial = serial;
    m_firmware = firmware;
}

void SelectionWriter::set_device_clock(uint devtime, uint systime)
{
    m_devtime = devtime;
    m_systime = systime;
}
