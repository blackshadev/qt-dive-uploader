#ifndef DCCONTEXT_H
#define DCCONTEXT_H
#include "dcdevicedescriptor.h"
#include <vector>

class DCContext
{
public:
    DCContext();
    std::vector<DCDeviceDescriptor> getDescriptors();
};

#endif // DCCONTEXT_H
