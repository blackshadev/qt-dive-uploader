#include "dcdownloadcontext.h"
#include <stdexcept>
#include <QCoreApplication>
#include <libdivecomputer/parser.h>
#include "dive.h"

DCDownloadContext::DCDownloadContext(QObject *parent) : QThread(parent)
{
    m_loglevel = DC_LOGLEVEL_WARNING;
    m_context = NULL;
    m_port_name = new char[256];
    m_descriptor = NULL;
}

DCDownloadContext::~DCDownloadContext() {
    delete[] m_port_name;
    m_port_name = NULL;
    free_context();
}

void DCDownloadContext::new_context() {

    if(m_context != NULL) {
        free_context();
    }

    dc_context_new(&m_context);
    dc_context_set_logfunc(
        m_context,
        DCDownloadContext::logfunc,
        this
    );
    dc_context_set_loglevel(m_context, m_loglevel);

}

void DCDownloadContext::free_context() {
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

    emit ctx->log(loglevels[loglevel], msg);
}

void DCDownloadContext::run() {

    new_context();

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

    int all_events = DC_EVENT_WAITING|DC_EVENT_CLOCK|DC_EVENT_PROGRESS|DC_EVENT_DEVINFO|DC_EVENT_VENDOR;
    dc_device_set_events(device, all_events, [](dc_device_t* device, dc_event_type_t type, const void* data, void* userdata) {
        DCDownloadContext* ctx = (DCDownloadContext *)userdata;
        switch(type) {

            case DC_EVENT_DEVINFO:
            {
                dc_event_devinfo_t* devinfo = (dc_event_devinfo_t*)data;
                emit ctx->deviceInfo(devinfo->model, devinfo->serial, devinfo->firmware);
            }
            break;

            case DC_EVENT_WAITING:
            {
                emit ctx->waiting();
            }
            break;

            case DC_EVENT_CLOCK:
            {
                dc_event_clock_t* clock = (dc_event_clock_t*)data;
                emit ctx->clock(clock->devtime, clock->systime);
            }
            break;

            case DC_EVENT_PROGRESS:
            {
                dc_event_progress_t* prog = (dc_event_progress_t*)data;
                emit ctx->progress(prog->current, prog->maximum);
            }
            break;

            case DC_EVENT_VENDOR:
            {
                dc_event_vendor_t* vend = (dc_event_vendor_t*)data;
                emit ctx->vendor(vend->data, vend->size);

            }
            break;
        }

    }, this);


    dc_device_foreach(
        device,
        [](
            const unsigned char* data,
            unsigned int size,
            const unsigned char* fingerprint,
            unsigned  int fsize,
            void* userdata
        ) -> int {

            dc_parser_t* parser;
            dc_parser_new(&parser, device);

            DCDownloadContext* ctx = (DCDownloadContext *)userdata;
            qInfo("Got dive: %s", fingerprint);
            Dive d(parser, data, size);

            qInfo("dt: "  + d.divetime);

            dc_parser_destroy(parser);
            return 1;
        },
        this
    );

    dc_device_close(device);

}
