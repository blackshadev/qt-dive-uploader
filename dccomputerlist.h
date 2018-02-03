#ifndef DCCOMPUTERLIST_H
#define DCCOMPUTERLIST_H
#include <QObject>
#include <QModelIndex>
#include <QAbstractListModel>
#include <libdivecomputer/device.h>

class DCComputer : public QObject
{
public:
    DCComputer(const dc_descriptor_t* descr);
    const dc_descriptor_t* descriptor;
    QString vendor;
    QString product;
};

Q_DECLARE_METATYPE(DCComputer*)

class DCComputerList: public QAbstractListModel
{   Q_OBJECT

public:
    explicit DCComputerList(QObject *parent = 0);
    ~DCComputerList();

    int rowCount(const QModelIndex& parent = QModelIndex()) const;
    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const;
public slots:

    // Extra function to get the thing easily from outside Repeaters.
    DCComputer* get(int idx);
    void add(DCComputer* comp);

private:
    QList<DCComputer*> mComputers;
};

#endif // DCCOMPUTERLIST_H
