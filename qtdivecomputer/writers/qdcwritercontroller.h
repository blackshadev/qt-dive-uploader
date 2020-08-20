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


class QDCWriterController : public QObject, public DCWriterInterface
{

    Q_OBJECT
    Q_PROPERTY(unsigned int current READ getCurrent WRITE setCurrent NOTIFY progress)
    Q_PROPERTY(unsigned int maximum READ getMaximum WRITE setMaximum NOTIFY progress)
    Q_PROPERTY(QDeviceData device WRITE setDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor)
    Q_PROPERTY(QDCWriter *writer WRITE setWriter)

public:
    QDCWriterController(QObject *parent = NULL);
    ~QDCWriterController();
    void setWriter(QDCWriter *writer);
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev);
    void setDescriptor(DCDeviceDescriptor *descr);
    void setDescriptor(QDCDescriptor *descr);
    unsigned int getMaximum();
    void setMaximum(unsigned int);
    unsigned int getCurrent();
    void setCurrent(unsigned int);

signals:
    void progress(unsigned int current, unsigned int maximum);
    void started();
    void finished();

protected:
    DeviceData device;
    QDCWriter *writer = NULL;
    QDCDescriptor *descriptor = NULL;

private:
    unsigned int current = 0;
    unsigned int maximum = 0;
    QThread *workerThread;


    // DCWriterInterface interface
public slots:
    void write(QDCDive *dive);
    void write(DCDive *dive) override;
    void start() override;
    void end() override;
    void cancel() override;
};

#endif // QDCWRITERCONTROLLER_H
