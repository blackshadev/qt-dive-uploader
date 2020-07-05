#ifndef DCDIVESAMPLEEVENTSERIALIZER_H
#define DCDIVESAMPLEEVENTSERIALIZER_H
#include "optionalserializer.h"
#include "../entities/dcdivesampleevent.h"

class DCDiveSampleEventSerializer
{
public:
    void serialize(QJsonObject &obj, DCDiveSampleEvent *data);
};

#endif // DCDIVESAMPLEEVENTSERIALIZER_H
