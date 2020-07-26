#ifndef DCWRITERINTERFACE_H
#define DCWRITERINTERFACE_H
#include "../entities/dcdive.h"
#include "../devices/dcdeviceinterface.h"
#include "../descriptor/dcdevicedescriptor.h"

class DCWriterInterface
{
public:
    virtual void add(DCDive *dive) = 0;
    virtual void start() = 0;
    virtual void end() = 0;
    virtual void setDescriptor(DCDeviceDescriptor *descr) = 0;
    virtual void setDevice(DCDeviceInterface *dev) = 0;
};

#endif // DCWRITERINTERFACE_H