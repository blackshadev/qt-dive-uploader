#include "dccomputerlist.h"

DCComputer::DCComputer(dc_descriptor_t* descr)
{
    descriptor = descr;
    vendor = QString(dc_descriptor_get_vendor(descr));
    product = QString(dc_descriptor_get_product(descr));
    description = QString();

    description += vendor;
    description += " ";
    description += product;

}

DCComputerList::DCComputerList(QObject *parent) : QAbstractListModel(parent)
{
}

int DCComputerList::rowCount(const QModelIndex& parent) const
{
    return mComputers.size();
}

QVariant DCComputerList::data(const QModelIndex& index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= mComputers.size())
        return QVariant(QVariant::Invalid);

    DCComputer* comp = mComputers[i];
    switch (role) {
    case VendorRole:
        return QVariant::fromValue(comp->vendor);
    case ProductRole:
        return QVariant::fromValue(comp->product);
    case DescriptionRole:
        return QVariant::fromValue(comp->description);
    }

    return QVariant(QVariant::Invalid);
}

DCComputer* DCComputerList::get(int idx)
{
    if (idx < 0 || idx >= mComputers.size())
        return nullptr;

    return mComputers[idx];
}

void DCComputerList::add(DCComputer *comp)
{
    mComputers.append(comp);
}

void DCComputerList::clear()
{
    for(DCComputer* c : mComputers) {
        delete c;
    }
    mComputers.clear();
}

QHash<int, QByteArray> DCComputerList::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[VendorRole] = "vendor";
    roles[ProductRole] = "product";
    roles[DescriptionRole] = "description";
    return roles;
}
