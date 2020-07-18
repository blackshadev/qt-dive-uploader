#include "qdcdevicedescriptorlistmodel.h"

QDCDeviceDescriptorListModel::QDCDeviceDescriptorListModel(QObject *parent)
    : QAbstractListModel(parent)
{}

QDCDeviceDescriptorListModel::~QDCDeviceDescriptorListModel()
{

}

QHash<int, QByteArray> QDCDeviceDescriptorListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[VendorRole] = "vendor";
    roles[ProductRole] = "product";
    roles[DescriptionRole] = "description";
    roles[TransportsRole] = "transports";
    return roles;
}

QVariant QDCDeviceDescriptorListModel::data(const QModelIndex& index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= items.size()) {
        return QVariant(QVariant::Invalid);
    }

    QDCDeviceDescriptor* descriptor = items[i];
    switch (role) {
        case VendorRole:
            return QVariant::fromValue(descriptor->getQVendor());
        case ProductRole:
            return QVariant::fromValue(descriptor->getQProduct());
        case DescriptionRole:
            return QVariant::fromValue(descriptor->getQDescription());
        case TransportsRole:
            return QVariant::fromValue(descriptor->getQTransports());
    }

    return QVariant(QVariant::Invalid);
}

void QDCDeviceDescriptorListModel::add(QDCDeviceDescriptor *descr)
{
    items.push_back(descr);
}

int QDCDeviceDescriptorListModel::rowCount(const QModelIndex& parent) const
{
    return items.size();
}
