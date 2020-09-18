#include "dcdivesampleeventserializer.h"

void DCDiveSampleEventSerializer::serialize(QJsonObject &obj, DCDiveSampleEvent *evt)
{
    obj["Type"] = (int)evt->getType();
    obj["Value"] = (int)evt->getValue();
    obj["Flags"] = (int)evt->getFlags();
}
