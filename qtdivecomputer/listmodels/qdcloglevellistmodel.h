#ifndef QDCLOGLEVELLISTMODEL_H
#define QDCLOGLEVELLISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include "../context/qdccontext.h"
#define LOG_LEVEL_COUNT 6

class QDCLogLevelListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum LogLevelRoles {
        DescriptionRole = Qt::UserRole + 1,
        ValueRole
    };
    Q_ENUM(LogLevelRoles)
    QDCLogLevelListModel(QObject *parent = NULL);
    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    const QDCContext::LogLevel items[LOG_LEVEL_COUNT];
};
Q_DECLARE_METATYPE(QDCLogLevelListModel *)

#endif // QDCLOGLEVELLISTMODEL_H
