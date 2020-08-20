#include "qdcwriter.h"

QDCWriter::QDCWriter(QObject *parent)
    : QObject(parent)
{
    connect(this, SIGNAL(doWrite(QDCDive *)), this, SLOT(write(QDCDive *)));
    connect(this, SIGNAL(doStart()), this, SLOT(start()));
    connect(this, SIGNAL(doCancel()), this, SLOT(cancel()));
    connect(this, SIGNAL(doEnd()), this, SLOT(end()));
}

QDCWriter::~QDCWriter()
{}

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

void QDCWriter::write(QDCDive *dive)
{
    write((DCDive *)dive);
}
