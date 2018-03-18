#ifndef DIVEWRITER_H
#define DIVEWRITER_H
#include "../dive.h"
#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QThread>
#include <QMutex>
#include <QQueue>
#include <QWaitCondition>

class DiveWriter: public QThread {
    Q_OBJECT
public:
    static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    static std::string format_datetime(dc_datetime_t* dt);

    DiveWriter();

    virtual void set_device_descriptor(dc_descriptor_t* descr);
    virtual void set_device_info(uint model, uint serial, uint firmware);
    virtual void set_device_clock(uint devtime, uint systime) ;
    virtual void add(Dive* d);
    virtual void start();
    virtual void end();
signals:
    void diveWritten(Dive* d);
    void progress(uint m_current, uint m_total);
protected:
    bool m_ended = false;
    bool m_busy = false;
    uint m_total = 0;
    uint m_current = 0;
    QWaitCondition m_wait_cond;
    //QMutex m_lock_wait;
    QMutex m_lock;
    QQueue<Dive*> m_queue;
    virtual void write(Dive* d) = 0;
    virtual void written(Dive* d);
    void run() override;

};

#endif // DIVEWRITER_H
