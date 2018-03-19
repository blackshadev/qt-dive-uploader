#include "littlelogwriter.h"

LittleLogWriter::LittleLogWriter(LittleDiveLog* log)
{
    m_littledivelog = log;
}

LittleLogWriter::~LittleLogWriter()
{}

void LittleLogWriter::set_device_info(uint model, uint serial, uint firmware)
{
    m_computer.firmware = firmware;
    m_computer.serial = serial;
}

void LittleLogWriter::set_device_descriptor(dc_descriptor_t *descr)
{
    m_computer.descr = descr;
}


void LittleLogWriter::start()
{
    // set to busy to prevent dives from writing
    m_busy = true;

    QObject computer;


    m_littledivelog->request(
        RequestMethod::POST,
        "/computer",
        JsonDiveWriter::write_computer(computer, m_computer.descr, m_computer.serial),
        [=](JsonResponse resp) {
            m_lock.lock();
            m_busy = false;
            m_lock.unlock();
            m_wait_cond.wakeOne();
        }
    );

    JsonDiveWriter::start();
}

void LittleLogWriter::end()
{
    DiveWriter::end();
}


void LittleLogWriter::write(Dive *d)
{

}
