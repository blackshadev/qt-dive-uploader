#include "dccomputerlist.h"

DCComputer::DCComputer(dc_descriptor_t* descr)
{
    descriptor = descr;
    vendor = QString(dc_descriptor_get_vendor(descr));
    product= QString(dc_descriptor_get_product(descr));
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

    return QVariant::fromValue(mComputers[i]);
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
