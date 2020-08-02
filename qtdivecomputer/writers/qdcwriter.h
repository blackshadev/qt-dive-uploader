#ifndef QDCWRITER_H
#define QDCWRITER_H
#include "../divecomputer/writers/dcwriterinterface.h"
#include "../descriptor/qdcdescriptor.h"
#include "../device/qdcdevice.h"
#include <QQueue>
#include <QThread>
#include <QFuture>

class QDCWriter : public QObject, public DCWriterInterface
{
    Q_OBJECT
    Q_PROPERTY(QDeviceData device WRITE setDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor)
    Q_PROPERTY(bool isStarted READ getIsStarted NOTIFY isBusyChanged)
    Q_PROPERTY(bool isReady READ isReady NOTIFY isReadyChanged)

public:
    QDCWriter(QObject *parent = NULL);
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void setDescriptor(QDCDescriptor *descr);
    void setIsStarted(bool busy);
    virtual bool isReady() = 0;
    bool isReadyForData() override;
signals:
    void started();
    void ended();
    void cancelled();
    void written();
    void isBusyChanged();
    void isReadyChanged();
    void readyForData();
    void error(QString msg);
    void progress(unsigned int current, unsigned int maximum);
protected:
    void writeCompleted();
    void setWriteReady(bool state);
    bool getIsStarted();
    DeviceData device;
    QDCDescriptor *descriptor = NULL;
private:
    bool isStarted = false;
    bool writeReady = false;
};

#endif // QDCWRITER_H
