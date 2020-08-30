#ifndef QDCWRITETARGET_H
#define QDCWRITETARGET_H
#include <QObject>
#include "../entities/qdcdive.h"
#include "../divecomputer/exporters/dcexporterinterface.h"
#include "../descriptor/qdcdescriptor.h"
#include "../device/qdcdevice.h"

class QDCWriteTarget : public QObject, public DCExporterInterface
{
    Q_OBJECT
    Q_PROPERTY(QDeviceData device WRITE setDevice READ getDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor READ getDescriptor)
    Q_PROPERTY(bool isBusy READ isBusy NOTIFY isBusyChanged)

public:
    QDCWriteTarget(QObject *parent = NULL);
    bool isBusy();

protected:
    void unsetBusy();
    void setBusy();

public slots:
    void write(DCDive *dive) override;
    virtual void write(QDCDive *dive) = 0;
    virtual void end() override = 0;
    virtual void cancel() override = 0;
    virtual void start() override = 0;

signals:
    void started();
    void ended();
    void cancelled();
    void error(QString msg);
    void written();
    void ready();
    void isBusyChanged();

public:
    QDeviceData getDevice();
    QDCDescriptor *getDescriptor();
    void setDevice(QDeviceData dev);
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void setDescriptor(QDCDescriptor *descr);
protected:
    void ensureNotBusy();
    void ensureBusy();
private:
    QDeviceData device;
    QDCDescriptor *descriptor;
    bool _isBusy = false;
};
Q_DECLARE_METATYPE(QDCWriteTarget *)

#endif // QDCWRITETARGET_H
