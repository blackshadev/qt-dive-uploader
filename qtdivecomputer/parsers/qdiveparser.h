#ifndef QDCDIVEPARSER_H
#define QDCDIVEPARSER_H
#include <QObject>
#include "../divecomputer/parsers/dcdiveparser.h"
#include "../entities/qdcdive.h"
#include "../device/qdcdevice.h"

class QDCDiveParser : public QObject, public DCDiveParser
{
    Q_OBJECT
    Q_PROPERTY(QDCDevice *device WRITE setDevice)
public:
    QDCDiveParser(QObject *parent = NULL);
    QDCDiveParser *setDevice(QDCDevice *device);
protected:
    DCDive *createDive() override;
};
Q_DECLARE_METATYPE(QDCDiveParser *)

#endif // QDCDIVEPARSER_H
