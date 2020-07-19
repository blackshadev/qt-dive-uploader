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
        USB = 2 << TransportType::USB,
        USBHID = 2 << TransportType::USBHID,
        Serial = 2 << TransportType::Serial,
        Bluetooth = 2 << TransportType::Bluetooth,
        IRDA = 2 << TransportType::IRDA,
        BLE  = 2 << TransportType::BLE
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
