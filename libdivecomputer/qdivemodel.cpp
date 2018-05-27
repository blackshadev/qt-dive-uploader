#include "qdivemodel.h"
#include <stdio.h>

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
    emit dataChanged(createIndex(row, 0), createIndex(row, 0), QVector<int>( { SelectedRole } ));
}

void QDiveModel::setSelectedAll(bool selected)
{
    for(auto d : m_dives) {
        d->ignore = !selected;
    }

    emit dataChanged(createIndex(0, 0), createIndex(m_dives.length() - 1, 0), QVector<int>( { SelectedRole } ));
}

int QDiveModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
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
            return QVariant(dive->display_datetime().data());
        case DiveTimeRole: {
            char str[30];
            int hours = dive->divetime / 3600;
            int minutes = dive->divetime / 60 % 60;
            int seconds = dive->divetime % 60;

            sprintf(str, "%02d:%02d:%02d", hours, minutes, seconds);
            return QString::fromLocal8Bit(str);

        }
        case DiveDepthRole: {
            char str[30];
            sprintf(str, "%.2f m", dive->maxDepth);
            return QString::fromLocal8Bit(str);
        }
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
