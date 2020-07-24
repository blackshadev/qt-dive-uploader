#include "qdcdescriptorlistmodel.h"
#include "../../common/vector.h"

QDCDescriptorListModel::QDCDescriptorListModel(QObject *parent)
    : QAbstractListModel(parent)
{}

QDCDescriptorListModel::~QDCDescriptorListModel()
{
    FREE_VECTOR_ITEMS(items)
            items.clear();
}

QHash<int, QByteArray> QDCDescriptorListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[VendorRole] = "vendor";
    roles[ProductRole] = "product";
    roles[DescriptionRole] = "description";
    roles[TransportsRole] = "transports";
    roles[DescriptorRole] = "descriptor";
    return roles;
}

QVariant QDCDescriptorListModel::data(const QModelIndex& index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= items.size()) {
        return QVariant(QVariant::Invalid);
    }

    QDCDescriptor* descriptor = items[i];
    switch (role) {
        case VendorRole:
            return QVariant::fromValue(descriptor->getQVendor());
        case ProductRole:
            return QVariant::fromValue(descriptor->getQProduct());
        case DescriptionRole:
            return QVariant::fromValue(descriptor->getQDescription());
        case TransportsRole:
            return QVariant::fromValue(descriptor->getQTransports());
        case DescriptorRole:
            return QVariant::fromValue(descriptor);
    }

    return QVariant(QVariant::Invalid);
}

void QDCDescriptorListModel::add(QDCDescriptor *descr)
{
    items.push_back(descr);
}

void QDCDescriptorListModel::loadDescriptors(QDCContext *ctx)
{
    beginResetModel();
    items.clear();
    dc_iterator_t *iter;
    dc_descriptor_t *descr;

    dc_descriptor_iterator(&iter);
    while (dc_iterator_next(iter, &descr) == DC_STATUS_SUCCESS) {
        items.push_back(new QDCDescriptor(descr, this));
    }

    dc_iterator_free(iter);

    endResetModel();
}

int QDCDescriptorListModel::rowCount(const QModelIndex& parent) const
{
    return items.size();
}
