#ifndef DCDIVECOMPUTERSERIALIZER_H
#define DCDIVECOMPUTERSERIALIZER_H
#include <QJsonObject>
#include "../qtdivecomputer/descriptor/qdcdescriptor.h"
#include "../qtdivecomputer/device/qdcdevice.h"

class DCDiveComputerSerializer
{
public:
    void serialize(QJsonObject &object, DeviceData device, QDCDescriptor *descriptor);
};

#endif // DCDIVECOMPUTERSERIALIZER_H
