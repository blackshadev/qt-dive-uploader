#include "qdctransportlistmodel.h"
#include "../divecomputer/transports/dctransportfactory.h"

QDCTransportListModel::QDCTransportListModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

QDCTransportListModel::~QDCTransportListModel()
{
    for (auto item : items) {
        delete item;
    }
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
        case TransportTypeRole:
            return QVariant::fromValue(transport->getQTransport());
        case TransportRole:
            return QVariant::fromValue(transport);
    }

    return QVariant(QVariant::Invalid);
}

void QDCTransportListModel::add(QDCTransport *descr)
{
    allItems.push_back(descr);
}

void QDCTransportListModel::loadTransports(QDCContext *ctx) {
    DCTransportFactory factory;
    factory.setContext(ctx);
    auto allTransports = getAllTransportTypes();
    auto supportedTypes = getSupportedTransports(ctx);

    for (int iX = 0; iX < TRANSPORTS_COUNT; iX++) {
        auto transportType = allTransports[iX];
        if(transportType & supportedTypes) {
            auto transport = factory.create(transportType);
            if (transport) {
                add(new QDCTransport(transport, this));
            }
        }
    }

}

void QDCTransportListModel::filter(QDCTransport::Types transportTypes)
{
    beginResetModel();
    transports_t transports = QDCTransport::translateTransports(transportTypes);

    items.clear();
    for (auto item : allItems) {
        auto itemTransport = item->getTransportType();
        if (transports & itemTransport) {
            items.push_back(item);
        }
    }

    endResetModel();
}

QHash<int, QByteArray> QDCTransportListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    roles[TransportTypeRole] = "transport";
    return roles;
}
