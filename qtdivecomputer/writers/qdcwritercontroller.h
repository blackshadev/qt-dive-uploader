#ifndef QDCWRITERCONTROLLER_H
#define QDCWRITERCONTROLLER_H
#include "../divecomputer/writers/dcwriterinterface.h"
#include "../descriptor/qdcdescriptor.h"
#include "../device/qdcdevice.h"
#include "../writers/qdcwriter.h"
#include "../entities/qdcdive.h"
#include <QQueue>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>

class QDCWriterController : public QThread, public DCWriterInterface
{
    Q_OBJECT
    Q_PROPERTY(unsigned int current READ getCurrent WRITE setCurrent NOTIFY progress)
    Q_PROPERTY(unsigned int maximum READ getMaximum WRITE setMaximum NOTIFY progress)
    Q_PROPERTY(QDeviceData device WRITE setDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor)
    Q_PROPERTY(QDCWriter *writer WRITE setWriter)
    Q_PROPERTY(bool isBusy READ getBusy NOTIFY isBusyChanged)

public:
    QDCWriterController(QObject *parent = NULL);
    ~QDCWriterController();
    void setWriter(QDCWriter *writer);
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void setDescriptor(QDCDescriptor *descr);
    unsigned int getMaximum();
    void setMaximum(unsigned int);
    unsigned int getCurrent();
    void setCurrent(unsigned int);
    bool getBusy();
    void setBusy(bool b);
    bool isWriteReady() override;
public slots:
    void write(DCDive *dive) override;
    void write(QDCDive *dive);
    void start() override;
    void end() override;
    void cancel() override;

signals:
    void progress(unsigned int current, unsigned int maximum);
    void finished();
    void cancelled();
    void started();
    void selectChanged(bool select);
    void isBusyChanged();
    void error(QString msg);
protected:
    virtual void process(DCDive *dive);
    void run() override;

    DeviceData device;
    QDCWriter *writer = NULL;
    QDCDescriptor *descriptor = NULL;
private:
    bool isBusy = false;
    bool isEnded = false;
    bool isCancelled = false;
    unsigned int current = 0;
    unsigned int maximum = 0;
    QMutex mutex;
    QWaitCondition queueNotEmpty;
    QWaitCondition readyForNextWrite;
    QQueue<DCDive *> queue;
    void waitForWriteReady(QMutex *mutex);
    void waitForQueueNotEmpty(QMutex *mutex);
};

#endif // QDCWRITERCONTROLLER_H
