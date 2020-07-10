#include "dcdevice.h"

DCDevice::DCDevice(DCContext *ctx)
{
    context = ctx;
    stream = NULL;
}

DCDevice::~DCDevice()
{
    if(stream) {
        dc_iostream_close(stream);
    }
}

dc_iostream_t *DCDevice::getNativeStream()
{
    if (!this->stream) {
        stream = loadNativeStream();
    }

    return stream;
}

DCContext *DCDevice::getContext() const {
     return context;
}

