#ifndef QDIVEMODEL_H
#define QDIVEMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>
#include "dive.h"

class QDiveModel : public QAbstractListModel
{
    Q_OBJECT
public:
    enum DiveRoles {
        SelectedRole = Qt::UserRole + 1,
        DateRole,
        DiveTimeRole,
        DiveDepthRole
    };
    QDiveModel(QObject *parent = 0);

    void addDive(Dive &dive);

    int rowCount(const QModelIndex & parent = QModelIndex()) const;

    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    protected:
        QHash<int, QByteArray> roleNames() const;
    private:
        QList<Dive> m_dives;

};

#endif // QDIVEMODEL_H
