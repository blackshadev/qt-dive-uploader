#ifndef QDCREADER_H
#define QDCREADER_H
#include "../divecomputer/reader/dcreader.h"
#include "../device/qdcdevice.h"
#include "../parsers/qdiveparser.h"
#include "../context/qdccontext.h"
#include <QThread>
#include <QObject>

class QDCReader : public QObject, public DCReader
{
    Q_OBJECT
    Q_PROPERTY(QDCDevice *device WRITE setDevice)
    Q_PROPERTY(QDCDiveParser *parser WRITE setParser)
    Q_PROPERTY(QDCContext *context WRITE setContext)
    Q_PROPERTY(bool isReady READ isReady NOTIFY isReadyChanged)

public:
    QDCReader(QObject *parent = NULL);
    QDCReader *setDevice(DCDeviceInterface *d) override;
    QDCReader *setParser(DCDiveParserInterface *p) override;
    QDCReader *setContext(DCContextInterface *ctx) override;

    Q_INVOKABLE virtual void setFingerprint(QByteArray data);

    void receiveError(const char *msg) override;
    void receiveDeviceInfoEvent(dc_event_devinfo_t *devInfo) override;
    void receiveProgressEvent(dc_event_progress_t *progress) override;
    void receiveWaitingEvent() override;
    void receiveClockEvent(dc_event_clock_t *clock) override;
    void receiveDive(DCDive *dive) override;
    void receiveFinished() override;

public slots:
    Q_INVOKABLE virtual void cancel() override;
    Q_INVOKABLE virtual void startReading() override;
signals:
    void isReadyChanged();
    void dive(QDCDive *dive);
    void progress(unsigned int current, unsigned int maximum);
    void deviceInfo(QDeviceData data);
    void clock(unsigned int deviceClock, dc_ticks_t systime);
    void waiting();
    void error(QString msg);
    void finished();
};
Q_DECLARE_METATYPE(QDCReader *)
#endif // QDCREADER_H
