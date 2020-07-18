#include "qdccontext.h"
#include "../divecomputer/transports/transporttype.h"
#include "../transports/qdctransport.h"
#include <QVector>

QDCContext::QDCContext(QObject *parent)
    : QObject(parent), DCContext()
{
    descriptorsModel = NULL;
    transportsModel = NULL;
}

QDCContext::~QDCContext()
{
    delete descriptorsModel;
}

QDCDeviceDescriptorListModel *QDCContext::getQDescriptorsListModel()
{
    if (descriptorsModel) {
        return descriptorsModel;
    }

    descriptorsModel = new QDCDeviceDescriptorListModel(this);
    auto descriptors = getDescriptors();

    for(auto descr : *descriptors) {
        descriptorsModel->add((QDCDeviceDescriptor *)descr);
    }

    return descriptorsModel;
}

QDCTransportListModel *QDCContext::getQTransportListModel()
{
    if (transportsModel) {
        return transportsModel;
    }

    transportsModel = new QDCTransportListModel(this);
    auto transports = getTransports();

    for(auto trans : *transports) {
        transportsModel->add(new QDCTransport(trans));
    }

    return transportsModel;
}

DCDeviceDescriptor *QDCContext::createDescriptor(dc_descriptor_t *descr)
{
    return new QDCDeviceDescriptor(descr, this);
}
