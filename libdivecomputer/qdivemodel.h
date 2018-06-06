#ifndef QDIVEMODEL_H
#define QDIVEMODEL_H

#include <QObject>
#include <QList>
#include <QAbstractListModel>
#include "dive.h"

class QDiveModel : public QAbstractListModel
{
    Q_OBJECT
    Q_PROPERTY(int selectedDiveCount READ selectedDiveCount NOTIFY selectedDivesChanged)

public:
    enum DiveRoles {
        SelectedRole = Qt::UserRole + 1,
        DateRole,
        DiveTimeRole,
        DiveDepthRole
    };

    Q_ENUMS(DiveRoles)

    QDiveModel(QObject *parent = 0);

    int selectedDiveCount() const;
    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;

    QHash<int, QByteArray> roleNames() const;


public slots:
    void clear();
    void add(Dive* dive);
    QVariant get(int index, int role = Qt::DisplayRole) const;
    void setSelected(int row, bool selected);
    void setSelectedAll(bool selected);

signals:
    void selectedDivesChanged();

    protected:
    private:
        QList<Dive*> m_dives;

};

#endif // QDIVEMODEL_H
