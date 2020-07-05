#ifndef DCDIVESAMPLESERIALIZER_H
#define DCDIVESAMPLESERIALIZER_H
#include "optionalserializer.h"
#include "dcdivesampleeventserializer.h"
#include "../entities/dcdivesample.h"

class DCDiveSampleSerializer
{
public:
    void serialize(QJsonObject &obj, DCDiveSample *data);
private:
    DCDiveSampleEventSerializer eventSerializer;
    OptionalSerializer optionalSerializer;
};

#endif // DCDIVESAMPLESERIALIZER_H
