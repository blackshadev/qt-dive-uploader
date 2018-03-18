#ifndef LITTLELOGWRITER_H
#define LITTLELOGWRITER_H
#include "./divewriter.h"
#include "./filewriter.h"
#include "../littledivelog/littledivelog.h"
#include <QQueue>
#include <QMutex>

class LittleLogWriter : public DiveWriter
{
public:
    LittleLogWriter(LittleDiveLog* log);
    ~LittleLogWriter() override;


    void set_device_descriptor(dc_descriptor_t* descr) override;
    void set_device_info(uint model, uint serial, uint firmware) override;

    void begin() override;
    void end() override;
    void write(Dive *d) override;
protected:
    LittleDiveLog* m_littledivelog;
    QQueue<Dive*> m_queue;
    QMutex m_lock;
};

#endif // LITTLELOGWRITER_H
