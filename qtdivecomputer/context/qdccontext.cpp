#include "qdccontext.h"
#include <QVector>

QDCContext::QDCContext(QObject *parent)
    : QObject(parent), DCContext()
{
    descriptorsModel = NULL;
}

QDCContext::~QDCContext()
{
    delete descriptorsModel;
}

QDCDeviceDescriptorListModel *QDCContext::getQDescriptors()
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

DCDeviceDescriptor *QDCContext::createDescriptor(dc_descriptor_t *descr)
{
    return new QDCDeviceDescriptor(descr, this);
}
