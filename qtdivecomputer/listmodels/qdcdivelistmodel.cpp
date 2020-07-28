#include "qdcdivelistmodel.h"
#include "../parsers/qdiveparser.h"
#include "../divecomputer/reader/dcreader.h"

QDCDiveListModel::QDCDiveListModel(QObject *parent) : QAbstractListModel(parent)
{

}

QDCDiveListModel::~QDCDiveListModel()
{

}

int QDCDiveListModel::rowCount(const QModelIndex &parent) const
{
    return items.size();
}

QVariant QDCDiveListModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= items.size()) {
        return QVariant(QVariant::Invalid);
    }

    auto dive = items[i];
    switch (role) {
        case DateRole:
            return QVariant::fromValue(QString::fromStdString(dive->getDisplayDateTime()));
        case DepthRole:
            return QVariant::fromValue(dive->getMaxDepth());
        case TimeRole:
            return QVariant::fromValue(QString::fromStdString(dive->getDisplayDivetime()));
        case SelectedRole:
            return QVariant::fromValue(isSelected(dive));
    }
}

std::vector<QDCDive *> *QDCDiveListModel::getData()
{
    return &items;
}

void QDCDiveListModel::add(QDCDive *dive)
{
    beginResetModel();
    items.push_back(dive);
    selected.insert(dive);
    endResetModel();
}

void QDCDiveListModel::clear()
{
    beginResetModel();
    for(auto item : items) {
        delete item;
    }
    items.clear();
    endResetModel();
}

void QDCDiveListModel::select(QDCDive *dive)
{
    selected.insert(dive);
}

void QDCDiveListModel::deselect(QDCDive *dive)
{
    selected.erase(dive);
}

bool QDCDiveListModel::isSelected(QDCDive *dive) const
{
    return selected.find(dive) != selected.end();
}

QHash<int, QByteArray> QDCDiveListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[DepthRole] = "depth";
    roles[TimeRole] = "time";
    roles[SelectedRole] = "selected";
    return roles;
}
