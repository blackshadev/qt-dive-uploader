#ifndef DIVEDOWNLOADER_H
#define DIVEDOWNLOADER_H

#include <QObject>
#include <QThread>
#include <libdivecomputer/common.h>
#include <libdivecomputer/context.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>
#include <libdivecomputer/parser.h>
#include "dive.h"

class DCDownloadContext : public QThread
{
    Q_OBJECT
public:
    explicit DCDownloadContext(QObject *parent = 0);
    ~DCDownloadContext();
    void setLogLevel(dc_loglevel_t err = DC_LOGLEVEL_ERROR);
    void setDescriptor(dc_descriptor_t* descriptor);
    void setFingerprint(const unsigned char* data, unsigned int size);
    dc_context_t* m_context;
signals:
    void log(QString loglevel, QString msg);
    void deviceInfo(uint model, uint serial, uint firmware);
    void clock(uint devtime, uint systime);
    void progress(uint current, uint total);
    void waiting();
    void vendor(QString data, uint size);
    void error(QString);
    void dive(Dive* d);
protected:
    void run() override;
    void do_work();
    dc_loglevel_t m_loglevel;
    dc_descriptor_t* m_descriptor;
    dc_iostream_t* m_iostream;
    dc_device_t *m_device;
private:
    void new_context();
    void free_context();
    static void logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata);
};

#endif // DIVEDOWNLOADER_H
