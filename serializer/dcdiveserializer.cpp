#include "dcdiveserializer.h"

void DCDiveSerializer::serialize(QJsonObject &json, DCDive *dive)
{
    json["date"] = QString::fromStdString(dive->getISODateTime());
    json["divetime"] = (int)dive->getDivetime();

    auto fingerprintData = dive->getFingerprint();
    json["fingerprint"] = serializeFingerprint(fingerprintData);
    json["max_depth"] = dive->getMaxDepth();
    json["min_temperature"] = dive->getMinTemperature();
    json["max_temperature"] = dive->getMaxTemperature();
    json["surface_temperature"] = dive->getSurfaceTemperature();
    json["atmospheric_pressure"] = dive->getAtmosphericPressure();

    auto divemode = dive->getDivemode();
    if (divemode.has_value) {
        json["mode"] = serializeDivemode(divemode.value);
    }

    auto salinity = dive->getSalinity();
    if (salinity.has_value) {
        QJsonObject salinityObj;
        json["salinity"]  = salinityObj;
        serializeSalinity(salinityObj, salinity.value);
    }

    QJsonArray tanksArray;
    json["tanks"] = tanksArray;
    for (auto tank : *(dive->getTanks())) {
        QJsonObject tankObj;
        serializeTank(tankObj, tank);
        tanksArray.append(tankObj);
    }

    QJsonArray samples;
    json["samples"] = samples;
    for (auto sample : *(dive->getSamples())) {
        QJsonObject sampleObj;
        sampleSerializer.serialize(sampleObj, sample);
    }

}

void DCDiveSerializer::serializeTank(QJsonObject &tankObj, tank_t &tank)
{
    QJsonObject presObj;
    tankObj["pressure"] = presObj;

    if (tank.pressures.has_value) {
        tankObj["volume"] = tank.pressures.value.volume;

        presObj["begin"] = tank.pressures.value.beginpressure;
        presObj["end"] = tank.pressures.value.endpressure;

        switch(tank.pressures.value.type) {
            case DC_TANKVOLUME_IMPERIAL:
                presObj["type"] = "psi";
            break;
            case DC_TANKVOLUME_METRIC:
                presObj["type"] = "bar";
            break;
            default:
                presObj["type"] = "bar";
            break;
        }
    }

    if (tank.gasmix.has_value) {
        tankObj["oxygen"] = tank.gasmix.value.oxygen * 100;
        tankObj["helium"] = tank.gasmix.value.helium * 100;
        tankObj["nitrogen"] = tank.gasmix.value.nitrogen * 100;
    }
}

QString DCDiveSerializer::serializeFingerprint(fingerprint_t &fingerprint)
{
    return QByteArray(
        reinterpret_cast<const char *>(fingerprint.data),
        fingerprint.size
    );
}

const char *DCDiveSerializer::serializeDivemode(divemode_t &divemode)
{
    switch(divemode) {
        case DC_DIVEMODE_OC: return "open circuit";
        case DC_DIVEMODE_CCR: return "closed circuit rebreather";
        case DC_DIVEMODE_SCR: return "semi-closed circuit rebreather";
        case DC_DIVEMODE_GAUGE: return "gauge";
        case DC_DIVEMODE_FREEDIVE: return "freedive";
    }
}

void DCDiveSerializer::serializeSalinity(QJsonObject &obj, salinity_t &salinity)
{
    switch (salinity.type) {
        case DC_WATER_SALT: obj["type"] = "salt"; break;
        case DC_WATER_FRESH: obj["type"] = "fresh"; break;
    }
    obj["density"] = salinity.density;
}
