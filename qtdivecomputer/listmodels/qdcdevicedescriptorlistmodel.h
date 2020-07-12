#ifndef QDCDEVICEDESCRIPTORLISTMODEL_H
#define QDCDEVICEDESCRIPTORLISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include "../entities/qdcdevicedescriptor.h"

class QDCDeviceDescriptorListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DeviceDescriptorRoles {
        VendorRole = Qt::UserRole + 1,
        ProductRole,
        DescriptionRole,
        IndexRole,
        TransportsRole,
    };
    Q_ENUMS(DeviceDescriptorRoles)
    QDCDeviceDescriptorListModel(QObject *parent = 0);
    virtual ~QDCDeviceDescriptorListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void add(QDCDeviceDescriptor *descr);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    std::vector<QDCDeviceDescriptor *> items;
};
Q_DECLARE_METATYPE(QDCDeviceDescriptorListModel *)

#endif // QDCDEVICEDESCRIPTORLISTMODEL_H
