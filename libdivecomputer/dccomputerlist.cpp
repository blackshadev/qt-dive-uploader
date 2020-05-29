#include "dccomputerlist.h"

DCComputer::DCComputer(int idx, dc_descriptor_t* descr)
{
    index = idx;
    descriptor = descr;
    vendor = new QString(dc_descriptor_get_vendor(descr));
    product = new QString(dc_descriptor_get_product(descr));
    description = new QString();
    transports = new DCTransportList(this);
    transports->loadTransports(dc_descriptor_get_transports(descr));

    description->append(vendor);
    description->append(" ");
    description->append(product);
}

DCComputer::~DCComputer() {
    index = -1;
    delete vendor;
    delete product;
    delete description;
    delete transports;

    product = NULL;
    vendor = NULL;
    description = NULL;
    descriptor = NULL;

    dc_descriptor_free(descriptor);
}

DCComputerList::DCComputerList(QObject*parent) : QAbstractListModel(parent)
{
}

DCComputerList::~DCComputerList()
{
    this->clear();
}

DCComputer* DCComputerList::addComputer(dc_descriptor_t* descr)
{
    auto comp = new DCComputer(mComputers.size(), descr);
    this->add(comp);
    return comp;
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
            return QVariant::fromValue(*(comp->vendor));
        case ProductRole:
            return QVariant::fromValue(*(comp->product));
        case DescriptionRole:
            return QVariant::fromValue(*(comp->description));
        case IndexRole:
            return QVariant::fromValue(comp->index);
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
    roles[IndexRole] = "index";
    return roles;
}
