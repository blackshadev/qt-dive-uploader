#include "dcdevicedescriptor.h"

DCDeviceDescriptor::DCDeviceDescriptor(dc_descriptor_t *descriptor)
{
    this->descriptor = descriptor;
}

DCDeviceDescriptor::~DCDeviceDescriptor()
{
    dc_descriptor_free(descriptor);
}

std::string DCDeviceDescriptor::getDisplayValue()
{
    std::string result;
    result.append(this->getVendorName());
    result.append(" ");
    result.append(this->getProductName());
    return result;
}

const char *DCDeviceDescriptor::getProductName()
{
    return dc_descriptor_get_product(this->descriptor);
}

const char *DCDeviceDescriptor::getVendorName()
{
    return dc_descriptor_get_vendor(this->descriptor);
}

unsigned int DCDeviceDescriptor::getModelNumber()
{
    return dc_descriptor_get_model(this->descriptor);
}

dc_family_t DCDeviceDescriptor::getFamilyType()
{
    return dc_descriptor_get_type(this->descriptor);
}

TransportFlag DCDeviceDescriptor::getTransports()
{
    return (TransportFlag) dc_descriptor_get_transports(this->descriptor);
}
