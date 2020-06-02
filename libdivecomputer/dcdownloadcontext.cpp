#include "dcdownloadcontext.h"
#include <stdexcept>
#include <QCoreApplication>
#include "dive.h"
#include <libdivecomputer/serial.h>
#include <libdivecomputer/bluetooth.h>
#include <libdivecomputer/irda.h>
#include <libdivecomputer/usbhid.h>
#include <libdivecomputer/ble.h>

DCDownloadContext::DCDownloadContext(QObject *parent) : QThread(parent)
{
    m_loglevel = DC_LOGLEVEL_WARNING;
    m_context = NULL;
    m_device = NULL;
    m_port = NULL;
    m_computer = NULL;
}

DCDownloadContext::~DCDownloadContext()
{
    free_context();
    m_port = NULL;
    m_computer = NULL;

    if(m_device)
    {
        dc_device_close(m_device);
        m_device = NULL;
    }
}

void DCDownloadContext::new_context()
{

    if(m_context != NULL)
    {
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

void DCDownloadContext::free_context()
{
    dc_context_free(m_context);
    m_context = NULL;
}

void DCDownloadContext::setFingerprint(const unsigned char* data, unsigned int len)
{
    dc_device_set_fingerprint(m_device, data, len);
}

void DCDownloadContext::setComputer(DCComputer *comp)
{
    m_computer = comp;
}

void DCDownloadContext::setPort(DCPort *port)
{
    m_port = port;
}

void DCDownloadContext::setLogLevel(dc_loglevel_t err)
{
    m_loglevel = err;
    dc_context_set_loglevel(m_context, m_loglevel);
}

void DCDownloadContext::logfunc (dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata)
{
    const char *loglevels[] = {"NONE", "ERROR", "WARNING", "INFO", "DEBUG", "ALL"};
    DCDownloadContext* ctx = (DCDownloadContext*)userdata;

    emit ctx->log(QString(loglevels[loglevel]), QString(msg));
}

void DCDownloadContext::run() {
    try {
        do_work();
    } catch(const std::invalid_argument& exp) {
        emit error(QString(exp.what()));
    } catch(const std::runtime_error& exp) {
        emit error(QString(exp.what()));
    }
}

dc_status_t open_io_device(dc_iostream_t** iostream, dc_context_t* ctx, DCPort* port)
{
    switch(port->transport)
    {
        case DC_TRANSPORT_BLE:
        case DC_TRANSPORT_BLUETOOTH:
        {
            auto addr = dc_bluetooth_device_get_address((dc_bluetooth_device_t*)port->device);
            //return dc_bluetooth_open(iostream, ctx, addr);
            return DC_STATUS_UNSUPPORTED;
        }
        case DC_TRANSPORT_USBHID:
        case DC_TRANSPORT_USB:
            return dc_usbhid_open(iostream, ctx, (dc_usbhid_device_t*)port->device);
        case DC_TRANSPORT_IRDA:
            return dc_irda_open(iostream, ctx, dc_irda_device_get_address((dc_irda_device_t*)port->device), 0);
        case DC_TRANSPORT_SERIAL:
            return dc_serial_open(iostream, ctx, dc_serial_device_get_name((dc_serial_device_t*)port->device));
        case DC_TRANSPORT_NONE:
            return DC_STATUS_UNSUPPORTED;

    }
}

void DCDownloadContext::do_work() {

    new_context();

    if(m_computer == NULL)
    {
        throw std::invalid_argument("No computer selected");
    }
    if(m_port == NULL)
    {
        throw std::invalid_argument("No port selected");
    }
    if(m_context == NULL)
    {
        throw std::invalid_argument("Context not initialized. Constructor not called or destructor was already called");
    }

    dc_status_t status;
    dc_iostream_t* iostream;

    status = open_io_device(&iostream, m_context, m_port);
    if(status != DC_STATUS_SUCCESS) {
        throw std::runtime_error("Failed to open io device");
    }

    status = dc_device_open(&m_device, m_context, m_computer->descriptor, iostream);
    if(status != DC_STATUS_SUCCESS) {
        throw std::runtime_error("Failed to open device");
    }

    int all_events = DC_EVENT_WAITING|DC_EVENT_CLOCK|DC_EVENT_PROGRESS|DC_EVENT_DEVINFO|DC_EVENT_VENDOR;
    dc_device_set_events(m_device, all_events, [](dc_device_t* device, dc_event_type_t type, const void* data, void* userdata) {
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
                emit ctx->vendor(QString((const char*)vend->data), vend->size);
            }
                break;
        }

    }, this);

    dc_device_foreach(
        m_device,
        [](
            const unsigned char* data,
            unsigned int size,
            const unsigned char* fingerprint,
            unsigned int fsize,
            void* userdata
        ) -> int {

            DCDownloadContext* ctx = (DCDownloadContext *)userdata;

            dc_parser_t* parser;
            auto st = dc_parser_new(&parser, ctx->m_device);
            if(st != DC_STATUS_SUCCESS) {
                throw std::runtime_error("Failed to create parser, got status: " + st);
            }
            dc_parser_set_data(parser, data, size);

            Dive* d = new Dive();
            d->parse(parser);
            d->set_fingerprint(fingerprint, fsize);

            emit ctx->dive(d);

            dc_parser_destroy(parser);
            parser = NULL;

            return 1;
        },
        this
    );

    dc_iostream_close(iostream);
    dc_device_close(m_device);
    m_device = NULL;
    iostream = NULL;
}
