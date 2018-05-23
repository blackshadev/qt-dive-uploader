#include "qdivemodel.h"

QDiveModel::QDiveModel(QObject *parent)
    : QAbstractListModel(parent)
{

}

void QDiveModel::clear()
{
   m_dives.clear();
}

void QDiveModel::add(Dive* dive) {
    m_dives.append(dive);
}

void QDiveModel::setSelected(int row, bool selected)
{
    m_dives[row]->ignore = !selected;
}

int QDiveModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    qInfo(" here ");
    return m_dives.count();
}

QVariant QDiveModel::get(int row, int role) const {
    return data(index(row), role);
}

QVariant QDiveModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_dives.count())
        return QVariant();

    const Dive *dive = m_dives[index.row()];
    switch(role) {
        case SelectedRole:
            return QVariant(!dive->ignore);
        case DateRole:
            return QVariant(dive->formatted_datetime().data());
        case DiveTimeRole:
            return QVariant(dive->divetime);
        case DiveDepthRole:
            return QVariant(dive->maxDepth);
        default:
            return QVariant();

    }
}

QHash<int, QByteArray> QDiveModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[SelectedRole] = "selected";
    roles[DateRole] = "datetime";
    roles[DiveDepthRole] = "depth";
    roles[DiveTimeRole] = "time";
    return roles;
}
