#ifndef QDCDEVICE_H
#define QDCDEVICE_H

#include <QObject>
#include "../divecomputer/devices/dcdeviceinterface.h"

class QDCDevice : public QObject, public DCDeviceInterface
{
    Q_OBJECT
public:
    QDCDevice(DCDeviceInterface *concreteType, QObject *parent = NULL);
    virtual ~QDCDevice();
    DCDeviceDescriptor *getDescriptor() const override;
    std::string getDescription() override;
    QString getQDescription();
    dc_device_t *getNative(DCContextInterface *ctx) override;

protected:
    DCDeviceInterface *concreteDevice;
};
Q_DECLARE_METATYPE(QDCDevice *)

#endif // QDCDEVICE_H
