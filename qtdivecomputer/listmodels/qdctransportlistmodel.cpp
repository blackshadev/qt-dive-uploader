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
            return QVariant::fromValue(descriptor->getQDescription());
    }

    return QVariant(QVariant::Invalid);
}

void QDCTransportListModel::add(QDCTransport *descr)
{
    items.push_back(descr);
}

QHash<int, QByteArray> QDCTransportListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    return roles;
}
