#include "dcdivesampleeventserializer.h"

void DCDiveSampleEventSerializer::serialize(QJsonObject &obj, DCDiveSampleEvent *evt)
{
    obj["Type"] = evt->getTypeName();
    obj["Value"] = (int)evt->getValue();
}
