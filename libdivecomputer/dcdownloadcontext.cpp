#include "dcdownloadcontext.h"
#include <stdexcept>

DCDownloadContext::DCDownloadContext(QObject *parent) : QObject(parent)
{
    m_loglevel = DC_LOGLEVEL_ALL;
    m_context = NULL;
    m_port_name = new char[256];
    m_descriptor = NULL;
}

DCDownloadContext::~DCDownloadContext() {
    delete m_port_name;
    m_port_name = NULL;
}

void DCDownloadContext::newContext() {

    if(m_context != NULL) {
        freeContext();
    }

    dc_context_new(&m_context);
    dc_context_set_logfunc(
        m_context,
        DCDownloadContext::logfunc,
        this
    );
    dc_context_set_loglevel(m_context, m_loglevel);

}

void DCDownloadContext::freeContext() {
    dc_context_free(m_context);
    m_context = NULL;
}

void DCDownloadContext::setDescriptor(dc_descriptor_t *descriptor) {
    m_descriptor = descriptor;
}

void DCDownloadContext::setPortName(const char *port_name) {
    // Copy it over, something changes the port_name afterwards
    strcpy(m_port_name, port_name);
}

void DCDownloadContext::setLogLevel(dc_loglevel_t err) {
    m_loglevel = err;
    dc_context_set_loglevel(m_context, m_loglevel);
}

void DCDownloadContext::logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata)
{
    const char *loglevels[] = {"NONE", "ERROR", "WARNING", "INFO", "DEBUG", "ALL"};
    DCDownloadContext* ctx = (DCDownloadContext*)userdata;


    if (loglevel == DC_LOGLEVEL_ERROR || loglevel == DC_LOGLEVEL_WARNING) {
        qWarning("%s: %s [in %s:%d (%s)]", loglevels[loglevel], msg, file, line, function);
    } else {
        qInfo("%s: %s", loglevels[loglevel], msg);
    }

    emit ctx->log(loglevels[loglevel], msg);
}

void DCDownloadContext::start() {

    qInfo("start with %s", m_port_name);
    newContext();

    if(m_context == NULL) {
        throw std::invalid_argument("Context not initialized. Constructor not called or destructor was already called");
    }
    if(m_port_name == NULL) {
        throw std::invalid_argument("No portname given");
    }
    if(m_descriptor == NULL) {
        throw std::invalid_argument("No device selected");
    }

    qInfo("Open device with %s", m_port_name);
    dc_device_t *device;
    dc_device_open(&device, m_context, m_descriptor, m_port_name);
    dc_device_set_events(device, DC_EVENT_DEVINFO, [](dc_device_t* device, dc_event_type_t type, const void* data, void* userdata) {
        DCDownloadContext* ctx = (DCDownloadContext *)userdata;
        dc_event_devinfo_t* devinfo = (dc_event_devinfo_t*)data;
        emit ctx->deviceInfo(devinfo->model, devinfo->serial, devinfo->firmware);
    }, this);

    dc_device_set_events(device, DC_EVENT_CLOCK, [](dc_device_t* device, dc_event_type_t type, const void* data, void* userdata) {
        DCDownloadContext* ctx = (DCDownloadContext *)userdata;
        dc_event_clock_t* clock = (dc_event_clock_t*)data;
        emit ctx->clock(clock->devtime, clock->systime);
    }, this);

    dc_device_set_events(device, DC_EVENT_CLOCK, [](dc_device_t* device, dc_event_type_t type, const void* data, void* userdata) {
        DCDownloadContext* ctx = (DCDownloadContext *)userdata;
        dc_event_progress_t* prog = (dc_event_progress_t*)data;
        emit ctx->progress(prog->current, prog->maximum);
    }, this);

    dc_device_set_events(device, DC_EVENT_WAITING, [](dc_device_t* device, dc_event_type_t type, const void* data, void* userdata) {
        DCDownloadContext* ctx = (DCDownloadContext *)userdata;
        qInfo("waiting");
    }, this);

    dc_device_foreach(
        device,
        [](
            const unsigned char* data,
            uint size,
            const unsigned char* fingerprint,
            uint fsize,
            void* userdata
        ) {
            DCDownloadContext* ctx = (DCDownloadContext *)userdata;
            qInfo("Got dive: %s", fingerprint);

        },
        this
    );

    dc_device_close(device);

}
