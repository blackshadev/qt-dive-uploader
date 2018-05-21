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
    DiveWriter();

    virtual void set_device_descriptor(dc_descriptor_t* descr);
    virtual void set_device_info(uint model, uint serial, uint firmware);
    virtual void set_device_clock(uint devtime, uint systime) ;
    virtual void add(Dive* d);
    void start();
    void end();
signals:
    void dive(Dive* d);
    void diveWritten(Dive* d);
    void progress(uint m_current, uint m_total);
    void error(QString msg);
    void ready();
    void starting();
    void done();
    void selectDives(QList<Dive> dives);
public slots:
    virtual void selectionDone(QList<Dive> dives);
protected:
    bool m_error = false;
    uint m_total = 0;
    uint m_current = 0;
    bool m_busy = false;
    bool m_ended = false;
    bool m_ready = false;
    QQueue<Dive*> m_queue;
    QMutex m_lock;
    virtual void check_more_work();
    virtual void do_work(Dive* d) = 0;
    virtual void work_done(Dive* d);
    virtual void do_start() = 0;
    virtual void do_end() = 0;
    void _teardown();
    void run() override;

};

#endif // DIVEWRITER_H
