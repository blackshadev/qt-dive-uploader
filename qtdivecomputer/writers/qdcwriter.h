#ifndef QDCWRITER_H
#define QDCWRITER_H
#include "../divecomputer/writers/dcwriterinterface.h"
#include "../descriptor/qdcdescriptor.h"
#include "../device/qdcdevice.h"
#include <QQueue>
#include <QThread>

class QDCWriter : public QObject, public DCWriterInterface
{
    Q_OBJECT
    Q_PROPERTY(QDeviceData device WRITE setDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor)
    Q_PROPERTY(bool isBusy READ getBusy NOTIFY isBusyChanged)

public:
    QDCWriter(QObject *parent = NULL);
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void setDescriptor(QDCDescriptor *descr);
    void setBusy(bool busy);
signals:
    void isBusyChanged();
protected:
    bool getBusy();
    DeviceData device;
    QDCDescriptor *descriptor = NULL;
    bool isBusy = false;
};

#endif // QDCWRITER_H
