#ifndef DIVEDOWNLOADER_H
#define DIVEDOWNLOADER_H

#include <QObject>
#include <libdivecomputer/common.h>
#include <libdivecomputer/context.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>

class DiveDownloader : public QObject
{
    Q_OBJECT
public:
    explicit DiveDownloader(QObject *parent = 0);
    ~DiveDownloader();
    void start();
    void setPortName(char * port);
    void setLogLevel(dc_loglevel_t err = DC_LOGLEVEL_ERROR);
    void setDescriptor(dc_descriptor_t* descriptor);
    void setFingerprint(char data[4]);
protected:
    char *m_port_name;
    dc_loglevel_t m_loglevel;
    dc_descriptor_t* m_descriptor;
    dc_context_t* m_context;
};

#endif // DIVEDOWNLOADER_H
