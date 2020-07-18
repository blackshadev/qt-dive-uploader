#include "qdctransport.h"
#include "../divecomputer/transports/transporttype.h"

QDCTransport::QDCTransport(DCTransportInterface *transport, QObject *parent)
    : QObject(parent)
{
    concreteTransport = transport;
}

TransportType QDCTransport::getTransportType()
{
    return concreteTransport->getTransportType();
}

std::vector<DCDeviceInterface *> *QDCTransport::getDevices(DCDeviceDescriptor *descriptor)
{
    return concreteTransport->getDevices(descriptor);
}

QString QDCTransport::getQDescription()
{
    return QString(transportTypeToString(getTransportType()));
}

QDCTransport::Type QDCTransport::getQTransport()
{
    return (Type)(2 << getTransportType());
}

QDCTransport::Types QDCTransport::translateTransports(transports_t trans)
{
    QDCTransport::Types types;
    if (trans & TransportType::BLE) {
        types |= QDCTransport::Type::BLE;
    }
    if (trans & TransportType::Bluetooth) {
        types |= QDCTransport::Type::Bluetooth;
    }
    if (trans & TransportType::USB) {
        types |= QDCTransport::Type::USB;
    }
    if (trans & TransportType::USBHID) {
        types |= QDCTransport::Type::USBHID;
    }
    if (trans & TransportType::IRDA) {
        types |= QDCTransport::Type::IRDA;
    }
    if (trans & TransportType::Serial) {
        types |= QDCTransport::Type::Serial;
    }
    return types;
}

transports_t QDCTransport::translateTransports(Types trans)
{
    transports_t types = 0;
    if (trans & QDCTransport::Type::BLE) {
        types |= TransportType::BLE;
    }
    if (trans & QDCTransport::Type::Bluetooth) {
        types |= TransportType::Bluetooth;
    }
    if (trans & QDCTransport::Type::USB) {
        types |= TransportType::USB;
    }
    if (trans & QDCTransport::Type::USBHID) {
        types |= TransportType::USBHID;
    }
    if (trans & QDCTransport::Type::IRDA) {
        types |= TransportType::IRDA;
    }
    if (trans & QDCTransport::Type::Serial) {
        types |= TransportType::Serial;
    }
    return types;
}


