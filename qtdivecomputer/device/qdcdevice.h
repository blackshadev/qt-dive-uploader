#ifndef QDCDEVICE_H
#define QDCDEVICE_H

#include <QObject>
#include "../divecomputer/devices/dcdeviceinterface.h"

class QDCDevice : public QObject, public DCDeviceInterface
{
    Q_OBJECT
public:
    QDCDevice(DCDeviceInterface *concreteType, QObject *parent = NULL);
    ~QDCDevice();
    DCDeviceDescriptor *getDescriptor() const;
    std::string getDescription();
    QString getQDescription();
    dc_device_t *getNative();
protected:
    DCDeviceInterface *concreteDevice;
};

#endif // QDCDEVICE_H
