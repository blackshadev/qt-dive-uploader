#ifndef DCDIVESERIALIZER_H
#define DCDIVESERIALIZER_H
#include "optionalserializer.h"
#include "dcdivesampleserializer.h"
#include "../divecomputer/entities/dcdive.h"

class DCDiveSerializer
{
public:
    void serialize(QJsonObject &obj, DCDive *data);
private:
    DCDiveSampleSerializer sampleSerializer;
    void serializeTank(QJsonObject &obj, tank_t &tank);
    QString serializeFingerprint(fingerprint_t &fingerprint);
    const char *serializeDivemode(divemode_t &divemode);
    void serializeSalinity(QJsonObject &obj, salinity_t &salinity);
};

#endif // DCDIVESERIALIZER_H
