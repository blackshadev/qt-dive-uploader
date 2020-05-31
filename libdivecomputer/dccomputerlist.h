#ifndef DCCOMPUTERLIST_H
#define DCCOMPUTERLIST_H
#include <QObject>
#include <QModelIndex>
#include <QAbstractListModel>
#include <libdivecomputer/device.h>
#include "dctransportlist.h"

class DCComputer : public QObject
{
public:
    DCComputer(int index, dc_descriptor_t* descr);
    virtual ~DCComputer();
    dc_descriptor_t* descriptor;
    QString* vendor;
    QString* product;
    QString* description;
    dc_transport_t transports;

    int index;
};

Q_DECLARE_METATYPE(DCComputer*)

class DCComputerList: public QAbstractListModel
{   Q_OBJECT

public:
    enum ComputerRoles {
        VendorRole = Qt::UserRole + 1,
        ProductRole,
        DescriptionRole,
        IndexRole,
    };
    Q_ENUMS(ComputerRoles)

    DCComputerList(QObject *parent = 0);
    virtual ~DCComputerList();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
    DCComputer* addComputer(dc_descriptor_t* descr);

public slots:

    // Extra function to get the thing easily from outside Repeaters.
    Q_INVOKABLE DCComputer* get(int idx);
    void add(DCComputer* comp);
    void clear();

protected:
    QHash<int, QByteArray> roleNames() const ;
private:
    QList<DCComputer*> mComputers;
};
#endif // DCCOMPUTERLIST_H
