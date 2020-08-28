#ifndef QDCWRITERCONTROLLER_H
#define QDCWRITERCONTROLLER_H
#include "../descriptor/qdcdescriptor.h"
#include "../device/qdcdevice.h"
#include "../entities/qdcdive.h"
#include "../exporters/qdcwritetarget.h"
#include <QQueue>
#include <QThread>
#include <QWaitCondition>
#include <QMutex>
#include "qdcwriterworker.h"

class QDCWriterController : public QObject
{

    Q_OBJECT
    Q_PROPERTY(unsigned int current READ getCurrent WRITE setCurrent NOTIFY progress)
    Q_PROPERTY(unsigned int maximum READ getMaximum WRITE setMaximum NOTIFY progress)
    Q_PROPERTY(QDeviceData device WRITE setDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor)
    Q_PROPERTY(QDCWriteTarget *writer WRITE setWriter)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)

public:
    QDCWriterController(QObject *parent = NULL);
    ~QDCWriterController();

    void setWriter(QDCWriteTarget *writer);
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev);
    void setDescriptor(DCDeviceDescriptor *descr);
    void setDescriptor(QDCDescriptor *descr);
    unsigned int getMaximum();
    void setMaximum(unsigned int);
    unsigned int getCurrent();
    void setCurrent(unsigned int);

public:
    bool isBusy();

signals:
    void progress(unsigned int current, unsigned int maximum);
    void cancelled();
    void ended();
    void error(QString);
    void started();
    void finished();
    void isBusyChanged();

private:
    unsigned int current = 0;
    unsigned int maximum = 0;
    QThread *workerThread;
    QDCWriteTarget *writeTarget = NULL;
    QDCWriterWorker *writerWorker = NULL;

    // DCWriterInterface interface
public slots:
    void write(QDCDive *dive);
    void write(DCDive *dive);
    void start();
    void end();
    void cancel();
};

#endif // QDCWRITERCONTROLLER_H
