#ifndef JSONRESPONSEINTERFACE_H
#define JSONRESPONSEINTERFACE_H
#include "responseinterface.h"
#include <QJsonObject>

class JSONResponseInterface : public ResponseInterface {
public:
    virtual QJsonObject getBodyAsJSON() = 0;
};


#endif // JSONRESPONSEINTERFACE_H
