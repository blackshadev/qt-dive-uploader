#ifndef QDCCONTEXT_H
#define QDCCONTEXT_H
#include <QObject>
#include <QList>
#include "../divecomputer/context/dccontext.h"
#include "../descriptor/qdcdevicedescriptor.h"
#include "../listmodels/qdcdevicedescriptorlistmodel.h"
#include "../listmodels/qdctransportlistmodel.h"

class QDCContext : public QObject, public DCContext
{
    Q_OBJECT
    Q_PROPERTY(QDCDeviceDescriptorListModel *descriptors READ getQDescriptorsListModel CONSTANT)
    Q_PROPERTY(QDCTransportListModel *transports READ getQTransportListModel CONSTANT)
public:
    QDCContext(QObject *parent = NULL);
    ~QDCContext();

    QDCDeviceDescriptorListModel *getQDescriptorsListModel();
    QDCTransportListModel *getQTransportListModel();
protected:
    QDCDeviceDescriptorListModel *descriptorsModel;
    QDCTransportListModel *transportsModel;
    DCDeviceDescriptor *createDescriptor(dc_descriptor_t *descr) override;
};

#endif // QDCCONTEXT_H
