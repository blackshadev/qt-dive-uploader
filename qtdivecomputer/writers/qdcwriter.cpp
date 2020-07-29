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

bool QDCWriter::getBusy()
{
    return isBusy;
}

void QDCWriter::setBusy(bool b)
{
    if (isBusy == b) {
        return;
    }

    isBusy = b;
    emit isBusyChanged();
}
