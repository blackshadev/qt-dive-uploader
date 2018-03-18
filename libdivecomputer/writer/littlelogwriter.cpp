#include "littlelogwriter.h"

LittleLogWriter::LittleLogWriter(LittleDiveLog* log)
{
    m_littledivelog = log;
}

LittleLogWriter::~LittleLogWriter()
{}

void LittleLogWriter::set_device_info(uint model, uint serial, uint firmware)
{

}

void LittleLogWriter::set_device_descriptor(dc_descriptor_t *descr)
{

}


void LittleLogWriter::begin()
{
    DiveWriter::begin();
}

void LittleLogWriter::end()
{
    DiveWriter::end();
}


void LittleLogWriter::write(Dive *d)
{

}
