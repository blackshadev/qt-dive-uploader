#include "qdcdevicedescriptor.h"

QDCDeviceDescriptor::QDCDeviceDescriptor(dc_descriptor_t *descriptor, QObject *parent)
    : QObject(parent), DCDeviceDescriptor(descriptor)
{

}

QString QDCDeviceDescriptor::getQDescription()
{
    return QString::fromStdString(getDisplayValue());
}

QString QDCDeviceDescriptor::getQVendor()
{
    return QString::fromStdString(getVendorName());
}

QString QDCDeviceDescriptor::getQProduct()
{
    return QString::fromStdString(getProductName());
}

QDCTransport::Types QDCDeviceDescriptor::getQTransports()
{
    return QDCTransport::translateTransports(getTransports());
}
