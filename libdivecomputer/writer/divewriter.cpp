#include "divewriter.h"

std::string DiveWriter::format_datetime(dc_datetime_t *dt) {
    char buff[25];
    std::sprintf(
        buff,
        "%04d-%02d-%02dT%02d:%02d:%02d+%02d:%02d",
        dt->year,
        dt->month,
        dt->day,
        dt->hour,
        dt->minute,
        dt->second,
        dt->timezone / 3600,
        dt->timezone % 3600
    );
    return std::string(buff);
}

DiveWriter::DiveWriter() : QThread(NULL) {
}

void DiveWriter::set_device_clock(uint devtime, uint systime) {}
void DiveWriter::set_device_descriptor(dc_descriptor_t *descr) {}
void DiveWriter::set_device_info(uint model, uint serial, uint firmware) {}

void DiveWriter::add(Dive *d)
{
    m_lock.lock();
    m_total += 1;
    m_queue.enqueue(d);
    m_lock.unlock();

    emit progress(m_current, m_total);
    check_more_work();
}

int iX = 0;

void DiveWriter::run()
{
    connect(this, &DiveWriter::dive, this, [this](Dive* d) { this->do_work(d); } );
    exec();
}

void DiveWriter::check_more_work()
{
    if(m_error) {
        exit(1);
        emit done();
        return;
    }

    if(m_busy || !m_ready) {
        return; // still busy
    }

    m_lock.lock();
    if(m_queue.length() > 0) {
        Dive *d = m_queue.dequeue();
        m_busy = true;
        m_lock.unlock();

        emit dive(d);
    } else if(m_ended) {
        // done
        m_lock.unlock();

        do_end();
    } else {
        m_lock.unlock();
    }
}

void DiveWriter::work_done(Dive *d)
{
    m_lock.lock();
    m_current += 1;
    m_busy = false;
    m_lock.unlock();

    emit progress(m_current, m_total);
    emit diveWritten(d);
    check_more_work();
}

void DiveWriter::start() {
    do_start();
    QThread::start();
}

void DiveWriter::end()
{
    m_lock.lock();
    m_ended = true;
    m_lock.unlock();
    check_more_work();
}

void DiveWriter::_teardown()
{
    emit done();
    exit(m_error == false);
}

