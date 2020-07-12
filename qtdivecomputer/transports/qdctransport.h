#ifndef QDCTRANSPORT_H
#define QDCTRANSPORT_H
#include <QObject>
#include "../divecomputer/transports/dctransportinterface.h"

class QDCTransport : public QObject, public DCTransportInterface
{
    Q_OBJECT
    Q_PROPERTY(QString description READ getQDescription CONSTANT)
public:
    QDCTransport(DCTransportInterface *transport, QObject *parent = NULL);
    TransportType getTransportType();
    std::vector<DCDeviceInterface *> *getDevices(DCDeviceDescriptor *descriptor);
    QString getQDescription();
protected:
    DCTransportInterface *concreteTransport;
};

#endif // QDCTRANSPORT_H
