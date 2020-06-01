#include "dcportlist.h"
#include <libdivecomputer/serial.h>
#include <libdivecomputer/ble.h>
#include <libdivecomputer/bluetooth.h>
#include <libdivecomputer/irda.h>
#include <libdivecomputer/usbhid.h>

DCPortList::DCPortList(QObject*parent) : QAbstractListModel(parent)
{

}

DCPortList::~DCPortList()
{

}


void DCPortList::clear()
{
    beginResetModel();
    for(DCPort *port : m_ports)
    {
            delete port;
    }
    m_ports.clear();
    endResetModel();
}


int DCPortList::rowCount(const QModelIndex& parent) const
{
    return m_ports.size();
}

QVariant DCPortList::data(const QModelIndex& index, int role) const
{
    int i = index.row();
    if ( i < 0 || i >= m_ports.size())
        return QVariant(QVariant::Invalid);

    DCPort* comp = m_ports[i];
    switch (role)
    {
        case DescriptionRole:
            return QVariant::fromValue(*(comp->description));
        case IndexRole:
            return QVariant::fromValue(comp->index);

    }

    return QVariant(QVariant::Invalid);
}

QHash<int, QByteArray> DCPortList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    roles[IndexRole] = "index";
    return roles;
}

dc_status_t DCPortList::load(dc_descriptor_t* descr,  dc_transport_t trans)
{
    clear();
    if(!(dc_descriptor_get_transports(descr) & trans))
    {
        return DC_STATUS_UNSUPPORTED;
    }



    return DC_STATUS_SUCCESS;
}

dc_status_t get_iterator_for(dc_iterator_t** iter, dc_transport_t trans)
{
    switch(trans) {
//        case DC_TRANSPORT_BLE: return dc_bluetooth_iterator_new(iter, m_con)
    }

    return DC_STATUS_UNSUPPORTED;
}
