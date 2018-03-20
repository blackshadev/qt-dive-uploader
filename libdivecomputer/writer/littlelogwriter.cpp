#include "littlelogwriter.h"
#include <functional>

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

    QJsonObject computer;

    JsonDiveWriter::write_computer(computer, m_computer.descr, m_computer.serial);

    m_littledivelog->request(
        RequestMethod::POST,
        "/computer",
        &computer,
        [=](JsonResponse resp) {
            if(resp.hasError()) {
                emit error(resp.errorString());
                m_lock.lock();
                m_error = true;
                m_lock.unlock();
                m_wait_cond.wakeOne();
                return;
            }
            auto obj = resp.data.object();
            m_computer_id = obj["computer_id"].toInt();
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

    written(d);
}
