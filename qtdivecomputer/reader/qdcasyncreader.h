#ifndef QDCASYNCREADER_H
#define QDCASYNCREADER_H
#include <QObject>
#include <QThread>
#include "../entities/qdcdive.h"
#include "../device/qdcdevice.h"
#include "../parsers/qdiveparser.h"
#include "../divecomputer/reader/dcreaderinterface.h"
#include "qdcreader.h"

class QDCAsyncReader : public QObject, public DCReaderInterface
{
    Q_OBJECT
    Q_PROPERTY(QDCDevice *device WRITE setDevice)
    Q_PROPERTY(QDCDiveParser *parser WRITE setParser)
public:
    QDCAsyncReader();
    ~QDCAsyncReader();

    DCReaderInterface *setDevice(DCDeviceInterface *device) override;
    DCReaderInterface *setParser(DCDiveParserInterface *parser) override;
    Q_INVOKABLE void start() override;

signals:
    void dive(QDCDive *dive);
    void progress(unsigned int current, unsigned int maximum);
    void deviceInfo(unsigned int model, unsigned int serial, unsigned int firmware);
    void clock(unsigned int deviceClock, dc_ticks_t systime);
    void waiting();
    void error(QString msg);

private:
    QThread workerThread;
    DCDeviceInterface* device;
    DCDiveParserInterface* parser;
    QDCReader* reader;
};
Q_DECLARE_METATYPE(QDCAsyncReader *)

#endif // QDCASYNCREADER_H
