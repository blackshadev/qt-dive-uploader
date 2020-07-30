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

bool QDCWriter::getIsBusy()
{
    return isBusy;
}

void QDCWriter::setIsBusy(bool b)
{
    if (isBusy == b) {
        return;
    }

    isBusy = b;
    emit isBusyChanged();
}

bool QDCWriter::isWriteReady()
{
    return writeReady;
}

void QDCWriter::readyForWrites()
{
    writeReady = true;
    emit isWriteReadyChanged();
}
