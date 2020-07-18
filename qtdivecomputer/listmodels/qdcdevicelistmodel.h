#ifndef QDCDEVICELISTMODEL_H
#define QDCDEVICELISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include "../device/qdcdevice.h"
#include "../divecomputer/transports/dctransportinterface.h"

class QDCDeviceListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    enum DeviceRoles {
        DescriptionRole = Qt::UserRole + 1,
    };
    Q_ENUM(DeviceRoles)
    QDCDeviceListModel(QObject *parent);
    virtual ~QDCDeviceListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void add(QDCDevice *descr);
    Q_INVOKABLE void loadDevices(DCTransportInterface *transport, DCDeviceDescriptor *descriptor);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    std::vector<QDCDevice *> items;
};
Q_DECLARE_METATYPE(QDCDeviceListModel *)

#endif // QDCDEVICELISTMODEL_H
