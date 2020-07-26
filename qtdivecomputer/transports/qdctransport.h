#ifndef QDCTRANSPORT_H
#define QDCTRANSPORT_H
#include <QObject>
#include "../divecomputer/transports/dctransportinterface.h"

class QDCTransport : public QObject, public DCTransportInterface
{
    Q_OBJECT
    Q_PROPERTY(QString description READ getQDescription CONSTANT)
public:
    enum Type {
        USB = 1 << TransportType::USB,
        USBHID = 1 << TransportType::USBHID,
        Serial = 1 << TransportType::Serial,
        Bluetooth = 1 << TransportType::Bluetooth,
        IRDA = 1 << TransportType::IRDA,
        BLE  = 1 << TransportType::BLE
    };
    Q_DECLARE_FLAGS(Types, Type)
    Q_FLAG(Types)
    Q_ENUM(Type)
    QDCTransport(DCTransportInterface *transport, QObject *parent = NULL);
    TransportType getTransportType();
    std::vector<DCDeviceInterface *> *getDevices(DCDeviceDescriptor *descriptor);
    QString getQDescription();
    Type getQTransport();
    static Types translateTransports(transports_t trans);
    static transports_t translateTransports(Types trans);
protected:
    DCTransportInterface *concreteTransport;
};
Q_DECLARE_METATYPE(QDCTransport *)
Q_DECLARE_OPERATORS_FOR_FLAGS(QDCTransport::Types)

#endif // QDCTRANSPORT_H
