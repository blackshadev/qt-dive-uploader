#ifndef DCSERIALDEVICE_H
#define DCSERIALDEVICE_H
#include "dcdevice.h"
#include <libdivecomputer/serial.h>


class DCSerialDevice  : public DCDevice
{
public:
    DCSerialDevice(DCDeviceDescriptor *descr, dc_serial_device_t *dev);
    ~DCSerialDevice();

    // DCDeviceInterface interface
public:
    std::string getDescription() override;

    // DCDevice interface
protected:
    dc_serial_device_t *device;
    dc_iostream_t *createNativeStream(DCContextInterface *ctx) override;
};

#endif // DCSERIALDEVICE_H
