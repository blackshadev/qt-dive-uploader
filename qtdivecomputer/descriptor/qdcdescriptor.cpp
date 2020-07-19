#include "qdcdescriptor.h"

QDCDescriptor::QDCDescriptor(dc_descriptor_t *descriptor, QObject *parent)
    : QObject(parent), DCDeviceDescriptor(descriptor)
{

}

QString QDCDescriptor::getQDescription()
{
    return QString::fromStdString(getDisplayValue());
}

QString QDCDescriptor::getQVendor()
{
    return QString::fromStdString(getVendorName());
}

QString QDCDescriptor::getQProduct()
{
    return QString::fromStdString(getProductName());
}

QDCTransport::Types QDCDescriptor::getQTransports()
{
    return QDCTransport::translateTransports(getTransports());
}
