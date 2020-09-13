#include "dcirdatransport.h"
#include "../devices/dcirdadevice.h"
#include <libdivecomputer/irda.h>
DCIRDATransport::DCIRDATransport(DCContextInterface *ctx) : DCTransport(ctx)
{

}

TransportType DCIRDATransport::getTransportType()
{
    return TransportType::IRDA;
}

std::vector<DCDeviceInterface *> *DCIRDATransport::getDevices(DCDeviceDescriptor *descriptor)
{
    dc_iterator_t *iter;
    dc_irda_device_t *device;
    auto devices = new std::vector<DCDeviceInterface *>();
    auto status = dc_irda_iterator_new(&iter, context->getNative(), descriptor->getNative());
    while (dc_iterator_next(iter, &device) == DC_STATUS_SUCCESS) {
        devices->push_back((DCDeviceInterface *)new DCIRDADevice(descriptor, device));
    }
    dc_iterator_free(iter);

    return devices;
}
