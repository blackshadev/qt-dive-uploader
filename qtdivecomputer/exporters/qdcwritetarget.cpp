#include "qdcwritetarget.h"


QDCWriteTarget::QDCWriteTarget(QObject *parent)
    : QObject(parent)
{}

QDeviceData QDCWriteTarget::getDevice()
{
    return device;
}

QDCDescriptor *QDCWriteTarget::getDescriptor()
{
    return descriptor;
}

void QDCWriteTarget::setDevice(QDeviceData dev)
{
    device = dev;
}

void QDCWriteTarget::setDevice(DeviceData dev)
{
    setDevice(static_cast<QDeviceData>(dev));
}

void QDCWriteTarget::setDescriptor(DCDeviceDescriptor *descr)
{
    setDescriptor(static_cast<QDCDescriptor *>(descr));
}

void QDCWriteTarget::setDescriptor(QDCDescriptor *descr)
{
    descriptor = descr;
}

void QDCWriteTarget::unsetBusy()
{
    _isBusy = false;
    emit isBusyChanged();
    emit ready();
}

void QDCWriteTarget::setBusy()
{
    _isBusy = true;
    emit isBusyChanged();
}

void QDCWriteTarget::write(DCDive *dive)
{
    write(static_cast<QDCDive *>(dive));
}

bool QDCWriteTarget::isBusy()
{
    return _isBusy;
}

void QDCWriteTarget::ensureNotBusy()
{
    if (isBusy()) {
        throw std::runtime_error("Already started");
    }
}

void QDCWriteTarget::ensureBusy()
{
    if (!isBusy()) {
        throw std::runtime_error("Not yet started");
    }
}
