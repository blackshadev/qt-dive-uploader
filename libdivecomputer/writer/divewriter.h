#ifndef DIVEWRITER_H
#define DIVEWRITER_H
#include "../dive.h"
#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class DiveWriter: public QObject {
    Q_OBJECT
public:
    static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    static std::string format_datetime(dc_datetime_t* dt);

    DiveWriter();

    virtual void set_device_descriptor(dc_descriptor_t* descr);
    virtual void set_device_info(uint model, uint serial, uint firmware);
    virtual void set_device_clock(uint devtime, uint systime) ;
    virtual void begin();
    virtual void end();
    virtual void write(Dive* d);
    virtual void written(Dive* d);
signals:
    void finished();
    void diveWritten(Dive* d);
    void progress(uint m_current, uint m_total);
protected:
    bool m_ended = false;
    uint m_total = 0;
    uint m_current = 0;
    virtual void done();
};

#endif // DIVEWRITER_H
