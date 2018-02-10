#ifndef DCCOMPUTERLIST_H
#define DCCOMPUTERLIST_H
#include <QObject>
#include <QModelIndex>
#include <QAbstractListModel>
#include <libdivecomputer/device.h>

class DCComputer : public QObject
{
public:
    DCComputer(dc_descriptor_t* descr);
    dc_descriptor_t* descriptor;
    QString vendor;
    QString product;
    QString description;
};

Q_DECLARE_METATYPE(DCComputer*)

class DCComputerList: public QAbstractListModel
{   Q_OBJECT

public:
    enum ComputerRoles {
        VendorRole = Qt::UserRole + 1,
        ProductRole,
        DescriptionRole,
        SelfRole,
    };
    Q_ENUMS(ComputerRoles)

    DCComputerList(QObject *parent = 0);

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;

public slots:

    // Extra function to get the thing easily from outside Repeaters.
    DCComputer* get(int idx);
    void add(DCComputer* comp);
    void clear();
protected:
    QHash<int, QByteArray> roleNames() const ;

private:
    QList<DCComputer*> mComputers;
};
#endif // DCCOMPUTERLIST_H
