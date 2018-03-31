#include "divewriter.h"

// source: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
static const std::string base64_chars =
             "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
             "abcdefghijklmnopqrstuvwxyz"
             "0123456789+/";
// source: https://renenyffenegger.ch/notes/development/Base64/Encoding-and-decoding-base-64-with-cpp
std::string DiveWriter::base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len) {
    std::string ret;
    int i = 0;
    int j = 0;
    unsigned char char_array_3[3];
    unsigned char char_array_4[4];

    while (in_len--) {
        char_array_3[i++] = *(bytes_to_encode++);
        if (i == 3) {
            char_array_4[0] = (char_array_3[0] & 0xfc) >> 2;
            char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
            char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);
            char_array_4[3] = char_array_3[2] & 0x3f;

            for(i = 0; (i <4) ; i++)
                ret += base64_chars[char_array_4[i]];
            i = 0;
        }
    }

    if (i)
    {
        for(j = i; j < 3; j++)
            char_array_3[j] = '\0';

        char_array_4[0] = ( char_array_3[0] & 0xfc) >> 2;
        char_array_4[1] = ((char_array_3[0] & 0x03) << 4) + ((char_array_3[1] & 0xf0) >> 4);
        char_array_4[2] = ((char_array_3[1] & 0x0f) << 2) + ((char_array_3[2] & 0xc0) >> 6);

        for (j = 0; (j < i + 1); j++)
            ret += base64_chars[char_array_4[j]];

        while((i++ < 3))
            ret += '=';

    }

    return ret;
}

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
    connect(this, &DiveWriter::dive, this, [this](Dive* d) { this->do_work(d); } );
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
    do_start();
    exec();
}

void DiveWriter::check_more_work()
{
    if(m_error) {
        exit(1);
        emit done();
        return;
    }

    if(m_busy) {
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
    QThread::start();
}

void DiveWriter::end()
{
    m_lock.lock();
    m_ended = true;
    m_lock.unlock();
    check_more_work();
}

void DiveWriter::do_end()
{
    emit done();
    exit(m_error == false);
}

void DiveWriter::do_start()
{
    emit starting();
}
