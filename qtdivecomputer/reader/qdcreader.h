#ifndef QDCREADER_H
#define QDCREADER_H
#include "../divecomputer/reader/dcreader.h"
#include "../device/qdcdevice.h"
#include "../parsers/qdiveparser.h"
#include <QThread>
#include <QObject>

class QDCReader : public QObject, public QThread, public DCReader
{
    Q_OBJECT
public:
    QDCReader(QObject *parent = NULL);
    QDCReader *setDevice(QDCDevice *d);
    QDCReader *setParser(QDiveParser *p);
    void run() override;
};
Q_DECLARE_METATYPE(QDCReader *)
#endif // QDCREADER_H
