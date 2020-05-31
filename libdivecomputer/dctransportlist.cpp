#include "dctransportlist.h"

DCTransport::DCTransport(int idx, dc_transport_t trans)
{
    index = idx;
    transport = trans;
    description = new QString(translate_transport(trans));
}

DCTransport::~DCTransport() {
    delete description;
    description = NULL;
    transport = DC_TRANSPORT_NONE;
}

const char* translate_transport(dc_transport_t trans)
{
    switch(trans) {
        case DC_TRANSPORT_BLE: return "Bluetooth Low Energie";
        case DC_TRANSPORT_BLUETOOTH: return "Bluetooth";
        case DC_TRANSPORT_IRDA: return "IRDA";
        case DC_TRANSPORT_SERIAL: return "Serial";
        case DC_TRANSPORT_USB: return "USB";
        case DC_TRANSPORT_USBHID: return "USB HID";
        default: return "Unknown";
    }
}


DCTransportList::DCTransportList(QObject*parent) : QAbstractListModel(parent)
{

}

DCTransportList::~DCTransportList()
{
    this->clear();
}

void DCTransportList::clear()
{
    beginResetModel();
    for(DCTransport* c : mTransports) {
        delete c;
    }
    mTransports.clear();
    endResetModel();

}

void DCTransportList::loadTransports(unsigned int transports)
{

    dc_transport_t const all_transports[6] = {
        DC_TRANSPORT_BLE,
        DC_TRANSPORT_BLUETOOTH,
        DC_TRANSPORT_IRDA,
        DC_TRANSPORT_SERIAL,
        DC_TRANSPORT_USB,
        DC_TRANSPORT_USBHID
    };

    clear();

    beginResetModel();
    for(dc_transport_t tran : all_transports) {
        if(transports & tran) {
            addTransport(tran);
        }
    }
    endResetModel();
}

DCTransport* DCTransportList::addTransport(dc_transport_t transport)
{
    auto trans = new DCTransport(mTransports.size(), transport);
    mTransports.append(trans);
    return trans;
}


int DCTransportList::rowCount(const QModelIndex& parent) const
{
    return mTransports.size();
}

QVariant DCTransportList::data(const QModelIndex& index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= mTransports.size())
        return QVariant(QVariant::Invalid);

    DCTransport* trans = mTransports[i];
    switch (role) {
        case DescriptionRole:
            return QVariant::fromValue(*(trans->description));
        case IndexRole:
            return QVariant::fromValue(trans->index);
    }

    return QVariant(QVariant::Invalid);

}

QHash<int, QByteArray> DCTransportList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    roles[IndexRole] = "index";
    return roles;
}
