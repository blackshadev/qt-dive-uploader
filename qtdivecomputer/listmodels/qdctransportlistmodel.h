#ifndef QDCTRANSPORTLISTMODEL_H
#define QDCTRANSPORTLISTMODEL_H
#include <QObject>
#include <QAbstractListModel>
#include "../transports/qdctransport.h"

class QDCTransportListModel : QAbstractListModel
{
    Q_OBJECT

public:
    enum TransportRoles {
        DescriptionRole = Qt::UserRole + 1,
    };
    Q_ENUMS(TransportRoles)
    QDCTransportListModel(QObject *parent = 0);
    virtual ~QDCTransportListModel();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    void add(QDCTransport *descr);
protected:
    QHash<int, QByteArray> roleNames() const;
private:
    std::vector<QDCTransport *> items;
};

#endif // QDCTRANSPORTLISTMODEL_H
