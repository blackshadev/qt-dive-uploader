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


QDeviceData::QDeviceData()
{
    model = 0;
    serial = 0;
    firmware = 0;
}

QDeviceData::QDeviceData(const DeviceData &data)
{
    model = data.model;
    serial = data.serial;
    firmware = data.firmware;
}
