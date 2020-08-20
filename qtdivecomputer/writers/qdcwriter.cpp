#include "qdcwriter.h"

QDCWriter::QDCWriter(QObject *parent)
    : QObject(parent)
{

}

void QDCWriter::setDevice(QDeviceData dev)
{
    setDevice((DeviceData)dev);
}

void QDCWriter::setDevice(DeviceData dev)
{
    device = dev;
}

void QDCWriter::setDescriptor(DCDeviceDescriptor *descr)
{
    setDescriptor((QDCDescriptor *)descr);
}

void QDCWriter::setDescriptor(QDCDescriptor *descr)
{
    descriptor = descr;
}

void QDCWriter::unsetBusy()
{
    _isBusy = false;
    emit ready();
}

void QDCWriter::setBusy()
{
    _isBusy = true;
}

bool QDCWriter::isBusy()
{
    return _isBusy;
}
