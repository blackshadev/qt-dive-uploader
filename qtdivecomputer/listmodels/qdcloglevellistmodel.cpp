#include "qdcloglevellistmodel.h"

QDCLogLevelListModel::QDCLogLevelListModel(QObject *parent)
    : items {
          QDCContext::LogLevel::None,
          QDCContext::LogLevel::Error,
          QDCContext::LogLevel::Warning,
          QDCContext::LogLevel::Info,
          QDCContext::LogLevel::Debug,
          QDCContext::LogLevel::All,
    }
{}

int QDCLogLevelListModel::rowCount(const QModelIndex &parent) const
{
    return LOG_LEVEL_COUNT;
}

QVariant QDCLogLevelListModel::data(const QModelIndex &index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= LOG_LEVEL_COUNT) {
        return QVariant(QVariant::Invalid);
    }

    auto ll = items[i];
    switch (role) {
        case DescriptionRole:
            return QVariant::fromValue(QDCContext::logLevelToString(ll));
        case ValueRole:
            return QVariant::fromValue(ll);
    }
}

QHash<int, QByteArray> QDCLogLevelListModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    roles[ValueRole] = "value";
    return roles;
}
