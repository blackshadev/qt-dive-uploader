#ifndef QDCCONTEXT_H
#define QDCCONTEXT_H
#include <QObject>
#include <QList>
#include "../divecomputer/context/dccontext.h"
#include "../entities/qdcdevicedescriptor.h"
#include "../listmodels/qdcdevicedescriptorlistmodel.h"

class QDCContext : public QObject, public DCContext
{
    Q_OBJECT
    Q_PROPERTY(QDCDeviceDescriptorListModel *descriptors READ getQDescriptors CONSTANT)
public:
    QDCContext(QObject *parent = NULL);
    ~QDCContext();

    QDCDeviceDescriptorListModel *getQDescriptors();
    DCDeviceDescriptor *createDescriptor(dc_descriptor_t *descr) override;
protected:
    QDCDeviceDescriptorListModel *descriptorsModel;
};

#endif // QDCCONTEXT_H
