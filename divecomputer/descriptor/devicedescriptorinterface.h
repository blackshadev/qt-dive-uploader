#ifndef DEVICEDESCRIPTORINTERFACE_H
#define DEVICEDESCRIPTORINTERFACE_H
#include <libdivecomputer/descriptor.h>
#include <string>

enum TransportFlag {

};

class DeviceDescriptorInterface
{
public:
    virtual std::string getDisplayValue() = 0;
    virtual const char *getProductName() = 0;
    virtual const char *getVendorName() = 0;
    virtual unsigned int getModelNumber() = 0;
    virtual dc_family_t getFamilyType() = 0;
    virtual TransportFlag getTransports() = 0;
};

#endif // DEVICEDESCRIPTORINTERFACE_H
