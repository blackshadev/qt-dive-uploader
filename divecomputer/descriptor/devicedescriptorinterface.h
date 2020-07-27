#ifndef DEVICEDESCRIPTORINTERFACE_H
#define DEVICEDESCRIPTORINTERFACE_H
#include "../transports/transporttype.h"
#include <libdivecomputer/descriptor.h>
#include <string>

class DeviceDescriptorInterface
{
public:
    virtual std::string getDisplayValue() = 0;
    virtual const char *getProductName() = 0;
    virtual const char *getVendorName() = 0;
    virtual unsigned int getModelNumber() = 0;
    virtual dc_family_t getFamilyType() = 0;
    virtual transports_t getTransports() = 0;

};

#endif // DEVICEDESCRIPTORINTERFACE_H
