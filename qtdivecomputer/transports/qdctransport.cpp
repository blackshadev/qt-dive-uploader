#include "qdctransport.h"
#include "../divecomputer/transports/transporttype.h"

QDCTransport::QDCTransport(DCTransportInterface *transport, QObject *parent)
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
