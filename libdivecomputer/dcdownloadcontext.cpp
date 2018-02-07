#include "dcdownloadcontext.h"
#include <stdexcept>

DCDownloadContext::DCDownloadContext(QObject *parent) : QObject(parent)
{

}

DCDownloadContext::~DCDownloadContext() {
}

void DCDownloadContext::newContext() {

    if(m_context != NULL) {
        freeContext();
    }

    dc_context_new(&m_context);
    dc_context_set_logfunc(m_context, DCDownloadContext::logfunc, NULL);
    dc_context_set_loglevel(m_context, m_loglevel);

}

void DCDownloadContext::freeContext() {
    dc_context_free(m_context);
    m_context = NULL;
}

void DCDownloadContext::setDescriptor(dc_descriptor_t *descriptor) {
    m_descriptor = descriptor;
}

void DCDownloadContext::setPortName(char *port_name) {
    m_port_name = port_name;
}

void DCDownloadContext::setLogLevel(dc_loglevel_t err) {
    m_loglevel = err;
    dc_context_set_loglevel(m_context, m_loglevel);
}

void DCDownloadContext::logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata)
{
    const char *loglevels[] = {"NONE", "ERROR", "WARNING", "INFO", "DEBUG", "ALL"};

    if (loglevel == DC_LOGLEVEL_ERROR || loglevel == DC_LOGLEVEL_WARNING) {
        qInfo("%s: %s [in %s:%d (%s)]\n", loglevels[loglevel], msg, file, line, function);
    } else {
        qInfo("%s: %s\n", loglevels[loglevel], msg);
    }
}

void DCDownloadContext::start() {

    if(m_context == NULL) {
        throw std::invalid_argument("Context not initialized. Constructor not called or destructor was already called");
    }
    if(m_port_name == NULL) {
        throw std::invalid_argument("No portname given");
    }
    if(m_descriptor == NULL) {
        throw std::invalid_argument("No device selected");
    }

    dc_device_t *device;
    dc_device_open(&device, m_context, m_descriptor, m_port_name);

    dc_device_close(device);
}
