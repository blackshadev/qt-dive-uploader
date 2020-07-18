#include "qdcdevicelistmodel.h"

QDCDeviceListModel::QDCDeviceListModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

int QDCDeviceListModel::rowCount(const QModelIndex &parent) const
{
    return items.size();
}

QVariant QDCDeviceListModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= items.size()) {
        return QVariant(QVariant::Invalid);
    }

    switch (role) {
        case DescriptionRole:
            return QVariant::fromValue(items[i]->getQDescription());
    }
}

void QDCDeviceListModel::add(QDCDevice *descr)
{
    items.push_back(descr);
}

void QDCDeviceListModel::clear()
{
    beginResetModel();

    for (auto device : items) {
        delete device;
    }
    items.clear();

    endResetModel();
}

void QDCDeviceListModel::loadDevices(DCTransportInterface *transport, DCDeviceDescriptor *descriptor)
{
    beginResetModel();

    auto devices = transport->getDevices(descriptor);
    for (auto device : *devices) {
        add(new QDCDevice(device, this));
    }
    delete devices;

    endResetModel();
}

QHash<int, QByteArray> QDCDeviceListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    return roles;
}
