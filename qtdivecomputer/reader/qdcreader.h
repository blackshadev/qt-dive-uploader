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
    Q_PROPERTY(bool isBusy READ getIsBusy NOTIFY isBusyChanged)
    Q_PROPERTY(unsigned int divesRead READ getDivesRead NOTIFY divesReadChanged)

public:
    QDCReader(QObject *parent = NULL);
    QDCReader *setDevice(DCDeviceInterface *d) override;
    QDCReader *setParser(DCDiveParserInterface *p) override;
    QDCReader *setContext(DCContextInterface *ctx) override;

    void receiveError(const char *msg) override;
    void receiveDeviceInfoEvent(dc_event_devinfo_t *devInfo) override;
    void receiveProgressEvent(dc_event_progress_t *progress) override;
    void receiveWaitingEvent() override;
    void receiveClockEvent(dc_event_clock_t *clock) override;
    void receiveDive(DCDive *dive) override;
    void receiveFinished() override;
    void receiveCancelled() override;
    void setFingerprint(fingerprint_t data) override;
    bool getIsBusy();
    unsigned int getDivesRead();

public slots:
    void cancel() override;
    void startReading() override;
    void setFingerprint(QByteArray data);
    void clearFingerprint() override;

signals:
    void isReadyChanged();
    void isBusyChanged();
    void dive(QDCDive *dive);
    void divesReadChanged();
    void progress(unsigned int current, unsigned int maximum);
    void deviceInfo(QDeviceData data);
    void clock(unsigned int deviceClock, dc_ticks_t systime);
    void waiting();
    void error(QString msg);
    void finished();
    void cancelled();
protected:
    void setIsBusy(bool b);
private:
    unsigned int divesRead = 0;
    bool isBusy = false;
};
Q_DECLARE_METATYPE(QDCReader *)
#endif // QDCREADER_H
