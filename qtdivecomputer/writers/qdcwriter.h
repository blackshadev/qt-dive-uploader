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

public:
    QDCWriter(QObject *parent = NULL);
    virtual ~QDCWriter();
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void setDescriptor(QDCDescriptor *descr);
    void unsetBusy();
    void setBusy();
    bool isBusy();

signals:
    void started();
    void ended();
    void cancelled();
    void error(QString msg);
    void written();
    void ready();

protected:
    bool _isBusy = false;
    DeviceData device;
    QDCDescriptor *descriptor = NULL;
};

#endif // QDCWRITER_H
