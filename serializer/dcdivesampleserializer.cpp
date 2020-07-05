#include "dcdivesampleserializer.h"


void DCDiveSampleSerializer::serialize(QJsonObject &obj, DCDiveSample *sample)
{
    obj["Time"] = (int)sample->getTime();
    optionalSerializer.serialize(obj, "Depth", sample->getDepth());
    optionalSerializer.serialize(obj, "Temperature", sample->getTemperature());
    optionalSerializer.serialize(obj, "CNS", sample->getCNS());
    optionalSerializer.serialize(obj, "SetPoint", sample->getSetpoint());
    optionalSerializer.serialize(obj, "PPO2", sample->getPPO2());
    optionalSerializer.serialize(obj, "Heartbeat", sample->getHeartbeat());
    optionalSerializer.serialize(obj, "RBT", sample->getRBT());

    if (sample->getPressures()->size()) {
        QJsonArray pressArr;
        obj["Pressure"] = pressArr;
        for (auto press : *(sample->getPressures())) {
            QJsonObject presObj;
            presObj["Tank"] = (int)press.tank;
            presObj["Pressure"] = press.pressure;
            pressArr.append(presObj);
        }
    }

    if (sample->getEvents()->size()) {
        QJsonArray eventsArr;
        obj["Events"] = eventsArr;
        for (auto event : *(sample->getEvents())) {
            QJsonObject eventObj;
            eventSerializer.serialize(eventObj, event);
            eventsArr.append(eventObj);
        }
    }
}
