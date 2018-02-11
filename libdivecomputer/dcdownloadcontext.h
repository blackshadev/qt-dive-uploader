#ifndef DIVEDOWNLOADER_H
#define DIVEDOWNLOADER_H

#include <QObject>
#include <libdivecomputer/common.h>
#include <libdivecomputer/context.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>

class DCDownloadContext : public QObject
{
    Q_OBJECT
public:
    explicit DCDownloadContext(QObject *parent = 0);
    ~DCDownloadContext();
    void start();
    void setPortName(char * port);
    void setLogLevel(dc_loglevel_t err = DC_LOGLEVEL_ERROR);
    void setDescriptor(dc_descriptor_t* descriptor);
    void setFingerprint(char data[4]);
signals:
    void log(const char* loglevel, const char* msg);
protected:
    char *m_port_name;
    dc_loglevel_t m_loglevel;
    dc_descriptor_t* m_descriptor;
    dc_context_t* m_context;
private:
    void newContext();
    void freeContext();
    static void logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata);
};

#endif // DIVEDOWNLOADER_H
