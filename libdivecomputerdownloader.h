#ifndef LIBDIVECOMPUTERDOWNLOADER_H
#define LIBDIVECOMPUTERDOWNLOADER_H

#include <QObject>
#include <libdivecomputer/common.h>
#include <libdivecomputer/context.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>

class LibDiveComputerDownloader : public QObject
{
    Q_OBJECT
public:
    explicit LibDiveComputerDownloader(QObject *parent = 0);
    ~LibDiveComputerDownloader();
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

#endif // LIBDIVECOMPUTERDOWNLOADER_H
