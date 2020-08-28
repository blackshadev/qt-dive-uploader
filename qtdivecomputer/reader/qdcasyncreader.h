#ifndef QDCASYNCREADER_H
#define QDCASYNCREADER_H
#include <QObject>
#include <QThread>
#include "../entities/qdcdive.h"
#include "../device/qdcdevice.h"
#include "../parsers/qdiveparser.h"
#include "../context/qdccontext.h"
#include "../divecomputer/reader/dcreaderinterface.h"
#include "qdcreader.h"

class QDCAsyncReader : public QObject, public DCReaderInterface
{
    Q_OBJECT
    Q_PROPERTY(QDCDevice *device WRITE setDevice)
    Q_PROPERTY(QDCDiveParser *parser WRITE setParser)
    Q_PROPERTY(QDCContext *context WRITE setContext)
    Q_PROPERTY(bool isBusy READ getIsBusy NOTIFY isBusyChanged)

public:
    QDCAsyncReader();
    ~QDCAsyncReader();

    DCReaderInterface *setDevice(DCDeviceInterface *device) override;
    DCReaderInterface *setParser(DCDiveParserInterface *parser) override;
    DCReaderInterface *setContext(DCContextInterface *context) override;
    bool getIsBusy();

public slots:
    void startReading() override;
    void cancel() override;
    void setFingerprint(QByteArray fingerprint);

signals:
    void dive(QDCDive *dive);
    void progress(unsigned int current, unsigned int maximum);
    void deviceInfo(QDeviceData data);
    void clock(unsigned int deviceClock, dc_ticks_t systime);
    void waiting();
    void error(QString msg);
    void cancelled();
    void finished();
    void startWork();
    void isBusyChanged();

private:
    bool isBusy = false;
    QThread *workerThread;
    DCDeviceInterface* device;
    DCDiveParserInterface* parser;
    DCContextInterface *context;
    QDCReader* reader;
};
Q_DECLARE_METATYPE(QDCAsyncReader *)

#endif // QDCASYNCREADER_H
