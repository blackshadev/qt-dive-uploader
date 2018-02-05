#include "libdivecomputerdownloader.h"

LibDiveComputerDownloader::LibDiveComputerDownloader(QObject *parent) : QObject(parent)
{
     qInfo("HERE");
}

LibDiveComputerDownloader::~LibDiveComputerDownloader() {
}

void LibDiveComputerDownloader::setDescriptor(dc_descriptor_t *descriptor) {
    m_descriptor = descriptor;
}

void LibDiveComputerDownloader::setPortName(char *port_name) {
    m_port_name = port_name;
}

void LibDiveComputerDownloader::setLogLevel(dc_loglevel_t err) {
    m_loglevel = err;
}

static void logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata)
{
    const char *loglevels[] = {"NONE", "ERROR", "WARNING", "INFO", "DEBUG", "ALL"};

    if (loglevel == DC_LOGLEVEL_ERROR || loglevel == DC_LOGLEVEL_WARNING) {
        qInfo("%s: %s [in %s:%d (%s)]\n", loglevels[loglevel], msg, file, line, function);
    } else {
        qInfo("%s: %s\n", loglevels[loglevel], msg);
    }
}

void LibDiveComputerDownloader::start() {
    dc_device_t *device;
    dc_context_t *context;

    dc_context_new(&context);
    dc_context_set_loglevel(context, m_loglevel);
    dc_context_set_logfunc(context, logfunc, NULL);

    dc_device_open(&device, context, m_descriptor, m_port_name);

    dc_device_close(device);
    dc_context_free(context);
}
