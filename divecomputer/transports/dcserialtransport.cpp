#include "dcserialtransport.h"
#include "../devices/dcserialdevice.h"
#include <libdivecomputer/serial.h>

DCSerialTransport::DCSerialTransport(DCContextInterface *ctx) : DCTransport(ctx)
{

}

TransportType DCSerialTransport::getTransportType()
{
    return TransportType::Serial;
}

std::vector<DCDeviceInterface *> *DCSerialTransport::getDevices(DCDeviceDescriptor *descriptor)
{

    dc_iterator_t *iter;
    dc_serial_device_t *device;
    auto devices = new std::vector<DCDeviceInterface *>();
    auto status = dc_serial_iterator_new(&iter, context->getNative(), descriptor->getNative());
    while (dc_iterator_next(iter, &device) == DC_STATUS_SUCCESS) {
        devices->push_back((DCDeviceInterface *)new DCSerialDevice(descriptor, device));
    }
    dc_iterator_free(iter);

    return devices;
}
