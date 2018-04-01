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


void LittleLogWriter::do_start()
{
    emit starting();
    QJsonObject computer;

    JsonDiveWriter::write_computer(computer, m_computer.descr, m_computer.serial);

    m_littledivelog->request(
        RequestMethod::POST,
        "/computer",
        &computer,
        [=](JsonResponse resp) {
            if(resp.hasError()) {
                emit error(resp.errorString());
                return;
            }
            auto obj = resp.data.object();
            m_computer_id = obj["computer_id"].toInt();

            m_lock.lock();
            m_ready = true;
            m_lock.unlock();

            emit ready();
            check_more_work();
        }
    );

}

void LittleLogWriter::do_end()
{
    _teardown();
}


void LittleLogWriter::do_work(Dive *d)
{
    QJsonObject data;

    JsonDiveWriter::write_dive(data, d);
    data["computer_id"] = m_computer_id;

    m_littledivelog->request(
        RequestMethod::POST,
        "/dive",
        &data,
        [=](JsonResponse resp) {
            if(resp.hasError()) {
                emit error(resp.errorString());
                m_error = true;
                work_done(d);
                return;
            }
            work_done(d);
        },
        this
    );
}
