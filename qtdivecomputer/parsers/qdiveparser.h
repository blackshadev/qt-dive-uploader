#ifndef QDCDIVEPARSER_H
#define QDCDIVEPARSER_H
#include <QObject>
#include "../divecomputer/parsers/dcdiveparser.h"
#include "../entities/qdcdive.h"
#include "../device/qdcdevice.h"
#include "../context/qdccontext.h"

class QDCDiveParser : public QObject, public DCDiveParser
{
    Q_OBJECT
    Q_PROPERTY(QDCDevice *device WRITE setDevice)
    Q_PROPERTY(QDCContext *context WRITE setContext)
public:
    QDCDiveParser(QObject *parent = NULL);
protected:
    DCDive *createDive() override;
};
Q_DECLARE_METATYPE(QDCDiveParser *)

#endif // QDCDIVEPARSER_H
