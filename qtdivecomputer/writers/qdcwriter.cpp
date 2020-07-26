#include "qdcwriter.h"

QDCWriter::QDCWriter(QObject *parent)
    : QObject(parent)
{

}

void QDCWriter::setDevice(QDCDevice *dev)
{
    device = dev;
}

void QDCWriter::setDescriptor(QDCDescriptor *descr)
{
    descriptor = descr;
}

void QDCWriter::setMaximum(unsigned int max)
{
    maximum = max;
    emit progress(current, maximum);
}

unsigned int QDCWriter::getMaximum()
{
    return maximum;
}

void QDCWriter::setCurrent(unsigned int cur)
{
    current = cur;
    emit progress(current, maximum);
}

unsigned int QDCWriter::getCurrent()
{
    return current;
}
