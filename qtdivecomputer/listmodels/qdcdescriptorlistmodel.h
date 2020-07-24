#ifndef QDCDEVICEDESCRIPTORLISTMODEL_H
#define QDCDEVICEDESCRIPTORLISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include "../descriptor/qdcdescriptor.h"
#include "../context/qdccontext.h"

class QDCDescriptorListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DescriptorRoles {
        VendorRole = Qt::UserRole + 1,
        ProductRole,
        DescriptionRole,
        TransportsRole,
        DescriptorRole
    };
    Q_ENUM(DescriptorRoles)
    QDCDescriptorListModel(QObject *parent = 0);
    virtual ~QDCDescriptorListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void add(QDCDescriptor *descr);
    Q_INVOKABLE void loadDescriptors(QDCContext *ctx);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    DCContextInterface *context;
    std::vector<QDCDescriptor *> items;
};
Q_DECLARE_METATYPE(QDCDescriptorListModel *)

#endif // QDCDEVICEDESCRIPTORLISTMODEL_H
