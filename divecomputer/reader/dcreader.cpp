#include "dcreader.h"
#include <stdexcept>
#include <libdivecomputer/device.h>

DCReader::DCReader()
{
    device = NULL;
    parser = NULL;
    callback = NULL;
}

DCReader *DCReader::setDevice(DCDevice *d)
{
    device = d;
    return this;
}

DCReader *DCReader::setCallback(dive_callback_t cb)
{
    callback = cb;
    return this;
}

DCReader *DCReader::setParser(DiveParser *p)
{
    parser = p;
    return this;
}

void DCReader::start()
{
    if (
        device == NULL ||
        parser == NULL ||
        callback == NULL
    ) {
        throw std::runtime_error("Not enough parameters. Device, Parser and Callback are required to be set");
    }

    auto dev = device->getNative();

    dc_device_foreach(dev, nativeCallback, this);
}

int DCReader::nativeCallback(const unsigned char *data, unsigned int size, const unsigned char *fpdata, unsigned int fsize, void *userdata)
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
    reader->callback(divedata);
}
