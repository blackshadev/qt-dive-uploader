#include "dcreader.h"
#include <stdexcept>
#include <libdivecomputer/device.h>

const unsigned int ALL_EVENTS = DC_EVENT_CLOCK | DC_EVENT_DEVINFO | DC_EVENT_WAITING | DC_EVENT_PROGRESS;

DCReader::DCReader()
{
    device = NULL;
    onDiveCallback = NULL;
}

DCReader *DCReader::setDevice(DCDevice *d)
{
    device = d;
    return this;
}

DCReader *DCReader::setParser(DiveParser *p)
{
    parser = p;
    return this;
}

DCReader *DCReader::setOnDiveCallback(dive_callback_t cb)
{
    onDiveCallback = cb;
    return this;
}

void DCReader::start()
{
    if (isReady()) {
        throw std::runtime_error("Reader not ready: not enough parameters.");
    }

    auto dev = device->getNative();

    dc_device_set_events(dev, ALL_EVENTS, nativeEventCallback, this);

    dc_device_foreach(dev, nativeDiveCallback, this);
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
    reader->onDiveCallback(dive);
}

void DCReader::nativeEventCallback(dc_device_t *device, dc_event_type_t event, const void *data, void *userdata)
{

}
