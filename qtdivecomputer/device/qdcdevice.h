#ifndef QDCDEVICE_H
#define QDCDEVICE_H
#include <QObject>
#include "../divecomputer/devices/dcdeviceinterface.h"

struct QDeviceData : public DeviceData {
    Q_GADGET
    Q_PROPERTY(unsigned int model MEMBER model)
    Q_PROPERTY(unsigned int serial MEMBER serial)
    Q_PROPERTY(unsigned int firmware MEMBER firmware)
public:
    QDeviceData();
    QDeviceData(const struct DeviceData&);
};
Q_DECLARE_METATYPE(QDeviceData)

class QDCDevice : public QObject, public DCDeviceInterface
{
    Q_OBJECT
public:
    QDCDevice(DCDeviceInterface *concreteType, QObject *parent = NULL);
    virtual ~QDCDevice();
    std::string getDescription() override;
    QString getQDescription();
    dc_device_t *getNative(DCContextInterface *ctx) override;

protected:
    DCDeviceInterface *concreteDevice;
};
Q_DECLARE_METATYPE(QDCDevice *)

#endif // QDCDEVICE_H
