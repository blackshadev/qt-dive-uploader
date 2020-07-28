#ifndef QDCDIVELISTMODEL_H
#define QDCDIVELISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include <set>
#include "../entities/qdcdive.h"
#include "../device/qdcdevice.h"

class QDCDiveListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DiveRoles {
        DateRole = Qt::UserRole + 1,
        DepthRole,
        TimeRole,
        SelectionRole,
    };
    Q_ENUM(DiveRoles)
    QDCDiveListModel(QObject *parent = NULL);
    virtual ~QDCDiveListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
public slots:
    void add(QDCDive *dive);
    void clear();
    void select(QDCDive *dive);
    void deselect(QDCDive *dive);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    std::vector<QDCDive *> items;
    std::set<QDCDive *> selected;
};
Q_DECLARE_METATYPE(QDCDiveListModel *)

#endif // QDCDIVELISTMODEL_H
