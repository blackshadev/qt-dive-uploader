#ifndef DIVEDOWNLOADER_H
#define DIVEDOWNLOADER_H

#include <QObject>
#include <QThread>
#include <libdivecomputer/common.h>
#include <libdivecomputer/context.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>

class DCDownloadContext : public QThread
{
    Q_OBJECT
public:
    explicit DCDownloadContext(QObject *parent = 0);
    ~DCDownloadContext();
    void setPortName(const char* port);
    void setLogLevel(dc_loglevel_t err = DC_LOGLEVEL_ERROR);
    void setDescriptor(dc_descriptor_t* descriptor);
    void setFingerprint(char data[4]);
signals:
    void log(const char* loglevel, const char* msg);
    void deviceInfo(uint model, uint serial, uint firmware);
    void clock(uint devtime, uint systime);
    void progress(uint current, uint total);
    void waiting();
    void vendor(const unsigned char* data, uint size);
protected:
    void run() override;
    char *m_port_name;
    dc_loglevel_t m_loglevel;
    dc_descriptor_t* m_descriptor;
    dc_context_t* m_context;
private:
    void new_context();
    void free_context();
    static void logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata);
};

#endif // DIVEDOWNLOADER_H
