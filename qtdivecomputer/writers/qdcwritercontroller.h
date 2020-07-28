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

public:
    QDCWriterController(QObject *parent = NULL);
    ~QDCWriterController();
    void setWriter(QDCWriter *writer);
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void setDescriptor(QDCDescriptor *descr);
    unsigned int getMaximum();
    void setCurrent(unsigned int);
    unsigned int getCurrent();
    void setMaximum(unsigned int);
public slots:
    void write(DCDive *dive) override;
    void write(QDCDive *dive);
    void start() override;
    void end() override;
signals:
    void progress(unsigned int current, unsigned int maximum);
    void finished();
    void selectChanged(bool select);
protected:
    virtual void process(DCDive *dive);
    void run() override;

    DeviceData device;
    DCWriterInterface *writer = NULL;
    QDCDescriptor *descriptor = NULL;
private:
    bool select = false;
    bool running = false;
    bool ended = false;
    QMutex mutex;
    QWaitCondition queueNotEmpty;
    QQueue<DCDive *> queue;
    unsigned int current = 0;
    unsigned int maximum = 0;
};

#endif // QDCWRITERCONTROLLER_H
