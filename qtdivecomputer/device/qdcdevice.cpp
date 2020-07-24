#include "qdcdevice.h"

QDCDevice::QDCDevice(DCDeviceInterface *device, QObject *parent)
    : QObject(parent)
{
    concreteDevice = device;
}

QDCDevice::~QDCDevice()
{
    delete concreteDevice;
}

DCDeviceDescriptor *QDCDevice::getDescriptor() const
{
    return  concreteDevice->getDescriptor();
}

std::string QDCDevice::getDescription()
{
    return concreteDevice->getDescription();
}

QString QDCDevice::getQDescription()
{
    return QString::fromStdString(concreteDevice->getDescription());
}

dc_device_t *QDCDevice::getNative(DCContextInterface *ctx)
{
    return concreteDevice->getNative(ctx);
}

