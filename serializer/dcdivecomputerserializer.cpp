#include "dcdivecomputerserializer.h"

void DCDiveComputerSerializer::serialize(QJsonObject &json, DeviceData device, QDCDescriptor *descriptor)
{
    json["serial"] =  (int)(device.serial);
    json["vendor"] = descriptor->getQVendor();
    json["model"] = (int)(descriptor->getModelNumber());
    json["type"] = (int)(descriptor->getFamilyType());
    json["name"] = QString(descriptor->getQProduct());
}
