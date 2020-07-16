#include "qdctransportlistmodel.h"

QDCTransportListModel::QDCTransportListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QDCTransportListModel::~QDCTransportListModel()
{
}

int QDCTransportListModel::rowCount(const QModelIndex &parent) const
{
    return items.size();
}

QVariant QDCTransportListModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= items.size())
        return QVariant(QVariant::Invalid);

    QDCTransport* transport = items[i];
    switch (role) {
        case DescriptionRole:
            return QVariant::fromValue(transport->getQDescription());
        case TransportRole:
            return QVariant::fromValue(transport->getQTransport());
    }

    return QVariant(QVariant::Invalid);
}

void QDCTransportListModel::add(QDCTransport *descr)
{
    allItems.push_back(descr);
}

void QDCTransportListModel::filter(transports_t transports)
{
    beginResetModel();

    items.clear();
    for (auto item : allItems) {
        if (transports & item->getTransportType()) {
            items.push_back(item);
        }
    }

    endResetModel();
}

QHash<int, QByteArray> QDCTransportListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    roles[TransportRole] = "transport";
    return roles;
}
