#include "dcportlist.h"
#include <libdivecomputer/serial.h>
#include <libdivecomputer/ble.h>
#include <libdivecomputer/bluetooth.h>
#include <libdivecomputer/irda.h>
#include <libdivecomputer/usbhid.h>

QString device_to_string(dc_transport_t trans, dc_device_t *device) {

    switch(trans)
    {
        case DC_TRANSPORT_USBHID:
        {
            int pid = dc_usbhid_device_get_pid((dc_usbhid_device_t *)device);
            int vid = dc_usbhid_device_get_vid((dc_usbhid_device_t *)device);
            return QString("[USB] pid: %1; vid: %2;").arg(pid).arg(vid);

            break;
        }
        case DC_TRANSPORT_IRDA:
        {
            auto name = dc_irda_device_get_name((dc_irda_device_t *)device);
            unsigned int addr = dc_irda_device_get_address((dc_irda_device_t *) device);

            return QString("[IRSA] %1; (address: %2)").arg(name).arg(addr);
        }
        case DC_TRANSPORT_SERIAL:
        {
            auto name = dc_serial_device_get_name((dc_serial_device_t *)device);

            return QString("[SERIAL] %1").arg(name);
        }
        case DC_TRANSPORT_BLUETOOTH:
        {
            auto name = dc_bluetooth_device_get_name((dc_bluetooth_device_t *)device);
            auto addr = dc_bluetooth_device_get_address((dc_bluetooth_device_t *)device);
            char str_addr[DC_BLUETOOTH_SIZE];
            dc_bluetooth_addr2str(addr, str_addr, DC_BLUETOOTH_SIZE);
            return QString("[BLUETOOTH] %1 (%2)").arg(name).arg(str_addr);
        }
        default:
            return QString("NONE");
    }

}

DCPort::DCPort(int idx, dc_transport_t trans, dc_device_t* dev)
{
    index = idx;
    transport = trans;
    device = dev;
    description = new QString(device_to_string(trans, dev));

}
DCPort::~DCPort()
{
    delete description;
}

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

DCPort* DCPortList::get(int idx) {
    return m_ports[idx];
}

QHash<int, QByteArray> DCPortList::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DescriptionRole] = "description";
    roles[IndexRole] = "index";
    return roles;
}

dc_status_t get_iterator_for(dc_iterator_t** iter, dc_context_t* ctx, dc_descriptor_t* descr, dc_transport_t trans)
{
    switch(trans) {
        case DC_TRANSPORT_BLE:
        case DC_TRANSPORT_BLUETOOTH:
            return dc_bluetooth_iterator_new(iter, ctx, descr);
        case DC_TRANSPORT_IRDA:
            return dc_irda_iterator_new(iter, ctx, descr);
        case DC_TRANSPORT_SERIAL:
            return dc_serial_iterator_new(iter, ctx, descr);
        case DC_TRANSPORT_USB:
        case DC_TRANSPORT_USBHID:
            return dc_usbhid_iterator_new(iter, ctx, descr);
    }

    return DC_STATUS_UNSUPPORTED;
}

dc_status_t DCPortList::load(dc_context_t* ctx, dc_descriptor_t* descr, dc_transport_t trans)
{
    clear();

    if(!(dc_descriptor_get_transports(descr) & trans))
    {
        return DC_STATUS_UNSUPPORTED;
    }

    dc_iterator_t *iter;
    dc_status_t status;

    status = get_iterator_for(&iter, ctx, descr, trans);

    if(status != DC_STATUS_SUCCESS)
    {
        return status;
    }

    void* device;

    beginResetModel();
    while((status = dc_iterator_next(iter, &device)) == DC_STATUS_SUCCESS)
    {
        m_ports.append(new DCPort(m_ports.size(), trans, (dc_device_t *)device));
    }
    endResetModel();

    dc_iterator_free(iter);

    emit loaded();

    return DC_STATUS_SUCCESS;
}


