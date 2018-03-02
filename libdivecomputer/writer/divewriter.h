#ifndef DIVEWRITER_H
#define DIVEWRITER_H
#include "../dive.h"
#include <QObject>
#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

class DiveWriter {
public:
    static std::string base64_encode(unsigned char const* bytes_to_encode, unsigned int in_len);
    DiveWriter();
    virtual ~DiveWriter() = 0;

    virtual void set_device_descriptor(dc_descriptor_t* descr);
    virtual void set_device_info(uint model, uint serial, uint firmware);
    virtual void set_device_clock(uint devtime, uint systime) ;
    virtual void begin() = 0;
    virtual void end() = 0;
    virtual void write(Dive* d) = 0;
    void (*onDone)() = NULL;
protected:
    virtual void done();
};

#endif // DIVEWRITER_H
