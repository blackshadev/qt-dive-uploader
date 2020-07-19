#ifndef QDCDIVELISTMODEL_H
#define QDCDIVELISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include "../entities/qdcdive.h"
#include "../device/qdcdevice.h"

class QDCDiveListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DiveRoles {
        DateRole = Qt::UserRole + 1,
        DepthRole,
        TimeRole
    };
    Q_ENUM(DiveRoles)
    QDCDiveListModel(QObject *parent = NULL);
    virtual ~QDCDiveListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void add(QDCDive *dive);
    Q_INVOKABLE void clear();
    Q_INVOKABLE void read(QDCDevice *device);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    std::vector<QDCDive *> items;
};
Q_DECLARE_METATYPE(QDCDiveListModel *)

#endif // QDCDIVELISTMODEL_H
