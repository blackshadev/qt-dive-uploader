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
        case DiveRole:
            return QVariant::fromValue(dive);
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
    for (auto item : items) {
        delete item;
    }
    items.clear();
    deselectAll();
    endResetModel();
}

void QDCDiveListModel::select(QDCDive *dive)
{
    auto idx = indexOf(dive);
    select(idx);
}

void QDCDiveListModel::select(const QModelIndex &index)
{
    QDCDive *dive = getDive(index);
    if (isSelected(dive)) {
        return;
    }

    selected.insert(dive);
    emit dataChanged(index, index, { DiveRoles::SelectedRole });
    emit selectedCountChanged(selectedCount());
}

void QDCDiveListModel::deselect(const QModelIndex &index)
{
    QDCDive *dive = getDive(index);
    if (!isSelected(dive)) {
        return;
    }

    selected.erase(dive);
    emit dataChanged(index, index, { DiveRoles::SelectedRole });
    emit selectedCountChanged(selectedCount());
}

void QDCDiveListModel::deselect(QDCDive *dive)
{
    auto idx = indexOf(dive);
    deselect(idx);
}

void QDCDiveListModel::toggle(const QModelIndex &index)
{
    toggle(index, isSelected(index));
}

void QDCDiveListModel::toggle(const QModelIndex &index, bool state)
{
    if (state) {
        select(index);
    } else {
        deselect(index);
    }
}

void QDCDiveListModel::toggle(QDCDive *dive)
{
    toggle(indexOf(dive), !isSelected(dive));
}

void QDCDiveListModel::toggle(QDCDive *dive, bool state)
{
    toggle(indexOf(dive), state);
}

bool QDCDiveListModel::isSelected(QDCDive *dive) const
{
    return selected.find(dive) != selected.end();
}

bool QDCDiveListModel::isSelected(const QModelIndex &index) const
{
    return isSelected(getDive(index));
}

void QDCDiveListModel::selectAll()
{

    beginResetModel();
    for (auto dive : items) {
        selected.insert(dive);
    }
    endResetModel();
    emit selectedCountChanged(selectedCount());
}

void QDCDiveListModel::deselectAll()
{
    beginResetModel();
    selected.clear();
    endResetModel();
    emit selectedCountChanged(selectedCount());
}

unsigned int QDCDiveListModel::selectedCount() const
{
    return selected.size();
}

QHash<int, QByteArray> QDCDiveListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DateRole] = "date";
    roles[DepthRole] = "depth";
    roles[TimeRole] = "time";
    roles[SelectedRole] = "selected";
    roles[DiveRole] = "dive";
    return roles;
}

QDCDive *QDCDiveListModel::getDive(const QModelIndex &index) const
{
    int i = index.row();
    if ( i < 0 || i >= items.size()) {
        throw std::runtime_error("Index out of bounds");
    }

    return items[i];
}

QModelIndex QDCDiveListModel::indexOf(QDCDive *dive) const
{
    unsigned int iX = 0;
    for (iX = 0; iX < items.size(); iX++) {
        if(items[iX] == dive) {
            return index(iX);
        }
    }

    throw std::runtime_error("Dive not found");
}
