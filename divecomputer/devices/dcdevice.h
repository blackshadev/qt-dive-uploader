#ifndef DCDEVICE_H
#define DCDEVICE_H
#include "../context/dccontext.h"
#include <libdivecomputer/device.h>

class DCDevice
{
public:
    DCDevice(DCContext *ctx);
    virtual ~DCDevice();
    virtual std::string getDescription() = 0;
    virtual dc_iostream_t *loadNativeStream() = 0;
    dc_iostream_t *getNativeStream();
    DCContext *getContext() const;
protected:
    DCContext *context;
    dc_iostream_t *stream;
};

#endif // DCDEVICE_H
