#include "dcreader.h"
#include <stdexcept>
#include <libdivecomputer/device.h>
#include <QThread>

const unsigned int ALL_EVENTS = DC_EVENT_CLOCK | DC_EVENT_DEVINFO | DC_EVENT_WAITING | DC_EVENT_PROGRESS | DC_EVENT_VENDOR;

DCReader::DCReader()
{
    device = NULL;
    parser = NULL;
    cancelled = false;
}

DCReader *DCReader::setDevice(DCDeviceInterface *d)
{
    device = d;
    return this;
}

DCReader *DCReader::setParser(DCDiveParserInterface*p)
{
    parser = p;
    return this;
}

bool DCReader::isReady()
{
    return parser != NULL && device != NULL;
}

void DCReader::start()
{

    if (!isReady()) {
        receiveError("Reader not ready.");
        return;
    }

    readAll();

}

void DCReader::readAll()
{
    auto dev = device->getNative();

    qInfo() << "readAll" << QThread::currentThreadId();
    dc_device_set_cancel(dev, nativeCancelCallback, this);
    dc_device_set_events(dev, ALL_EVENTS, nativeEventCallback, this);

    dc_device_foreach(dev, nativeDiveCallback, this);
    qInfo() << "done";
}

void DCReader::cancel()
{
    cancelled = true;
}

bool DCReader::isCancelled()
{
    return cancelled;
}

void DCReader::setFingerprint(fingerprint_t fp)
{
    auto dev = device->getNative();
    dc_device_set_fingerprint(dev, fp.data, fp.size);
}

void DCReader::receiveError(const char *msg)
{
    throw std::runtime_error(msg);
}

int DCReader::nativeDiveCallback(const unsigned char *data, unsigned int size, const unsigned char *fpdata, unsigned int fsize, void *userdata)
{
    auto reader = (DCReader *)userdata;
    rawdivedata_t divedata = {
        size,
        data,
        {
            fsize,
            fpdata
        }
    };

    auto dive = reader->parser->parseDive(divedata);
    reader->receiveDive(dive);

    return true;
}

void DCReader::nativeEventCallback(dc_device_t *device, dc_event_type_t event, const void *data, void *userdata)
{
    auto reader = (DCReader *)userdata;
    switch (event) {
        case DC_EVENT_CLOCK:
            reader->receiveClockEvent((dc_event_clock_t *)data);
        break;
        case DC_EVENT_DEVINFO:
            reader->receiveDeviceInfoEvent((dc_event_devinfo_t *)data);
        break;
        case DC_EVENT_WAITING:
            reader->receiveWaitingEvent();
        break;
        case DC_EVENT_PROGRESS:
            reader->receiveProgressEvent((dc_event_progress_t *)data);
        break;
        case DC_EVENT_VENDOR:
            /* NOOP */
        break;
    }
}

int DCReader::nativeCancelCallback(void *userdata)
{
    auto reader = (DCReader *)userdata;
    return reader->isCancelled();
}
