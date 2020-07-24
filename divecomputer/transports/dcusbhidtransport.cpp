#include "dctransport.h"
#include "../devices/dcusbhiddevice.h"
#include <libdivecomputer/usbhid.h>

DCUSBHIDTransport::DCUSBHIDTransport(DCContextInterface *ctx) : DCTransport(ctx)
{}

TransportType DCUSBHIDTransport::getTransportType()
{
    return TransportType::USBHID;
}

std::vector<DCDeviceInterface *> *DCUSBHIDTransport::getDevices(DCDeviceDescriptor *descriptor)
{
    dc_iterator_t *iter;
    dc_usbhid_device_t *device;
    auto devices = new std::vector<DCDeviceInterface *>();
    auto status = dc_usbhid_iterator_new(&iter, context->getNative(), descriptor->getNative());
    while (dc_iterator_next(iter, &device) == DC_STATUS_SUCCESS) {
        devices->push_back((DCDeviceInterface *)new DCUSBHIDDevice(descriptor, device));
    }
    dc_iterator_free(iter);

    return devices;
}
