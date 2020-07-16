#ifndef QDCDEVICEDESCRIPTOR_H
#define QDCDEVICEDESCRIPTOR_H
#include <QObject>
#include "../divecomputer/descriptor/dcdevicedescriptor.h"
#include "../transports/qdctransport.h"

class QDCDeviceDescriptor : public QObject, public DCDeviceDescriptor
{
    Q_OBJECT
    Q_PROPERTY(QString description READ getQDescription CONSTANT)
    Q_PROPERTY(QString vendor READ getQVendor CONSTANT)
    Q_PROPERTY(QString product READ getQProduct CONSTANT)
    Q_PROPERTY(QDCTransport::Types transports READ getQTransports CONSTANT)

public:
    QDCDeviceDescriptor(dc_descriptor_t *descriptor, QObject *parent = 0);
    QString getQDescription();
    QString getQVendor();
    QString getQProduct();
    QDCTransport::Types getQTransports();
};
Q_DECLARE_METATYPE(QDCDeviceDescriptor *)

#endif // QDCDEVICEDESCRIPTOR_H
