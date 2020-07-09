#ifndef DCDEVICEDESCRIPTOR_H
#define DCDEVICEDESCRIPTOR_H
#include "./devicedescriptorinterface.h"
#include <string>

class DCDeviceDescriptor : public DeviceDescriptorInterface
{
public:
    DCDeviceDescriptor(dc_descriptor_t *descriptor);
    ~DCDeviceDescriptor();
    std::string getDisplayValue();
    const char *getProductName();
    const char *getVendorName();
    unsigned int getModelNumber();
    dc_family_t getFamilyType();
    TransportFlag getTransports();
private:
    dc_descriptor_t *descriptor;
};

#endif // DCDEVICEDESCRIPTOR_H
