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

bool QDCWriter::getIsStarted()
{
    return isStarted;
}

void QDCWriter::setIsStarted(bool b)
{

    if (isStarted == b) {
        return;
    }

    isStarted = b;
    emit isBusyChanged();
}

bool QDCWriter::isReadyForData()
{
    return writeReady;
}

void QDCWriter::writeCompleted()
{
    emit written();
    setWriteReady(true);
}

void QDCWriter::setWriteReady(bool state)
{
    writeReady = state;
    if (writeReady) {
        emit readyForData();
    }
}
