#include "selectionwriter.h"

SelectionWriter::SelectionWriter(DiveWriter *writer)
{
    m_writer = writer;
}

void SelectionWriter::do_start()
{

}

void SelectionWriter::do_end()
{
    delete m_writer;
    m_writer = NULL;
}

void SelectionWriter::do_work(Dive *d)
{
    m_dives.append(*d);
}

void SelectionWriter::set_device_descriptor(dc_descriptor_t *descr)
{

}

void SelectionWriter::set_device_info(uint model, uint serial, uint firmware)
{

}

void SelectionWriter::set_device_clock(uint devtime, uint systime)
{

}
