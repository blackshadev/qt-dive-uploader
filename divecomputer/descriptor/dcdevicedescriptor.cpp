#include "dcdevicedescriptor.h"

DCDeviceDescriptor::DCDeviceDescriptor(dc_descriptor_t *descr)
{
    descriptor = descr;
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
    return dc_descriptor_get_product(descriptor);
}

const char *DCDeviceDescriptor::getVendorName()
{
    return dc_descriptor_get_vendor(descriptor);
}

unsigned int DCDeviceDescriptor::getModelNumber()
{
    return dc_descriptor_get_model(descriptor);
}

dc_family_t DCDeviceDescriptor::getFamilyType()
{
    return dc_descriptor_get_type(descriptor);
}

transports_t DCDeviceDescriptor::getTransports()
{
    return translateTransportsTypes(dc_descriptor_get_transports(descriptor));
}

dc_descriptor_t *DCDeviceDescriptor::getNative()
{
    return descriptor;
}
