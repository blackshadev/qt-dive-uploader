#include "qlibdivecomputer.h"
#include "dccomputerlist.h"
#include <libdivecomputer/version.h>
#include <libdivecomputer/device.h>
#include <QSerialPortInfo>


QLibDiveComputer::QLibDiveComputer(QObject *parent) : QObject(parent)
{
    m_last_context = NULL;
    m_available_devices = get_devices();
    m_available_portnames = get_ports();
    get_version();
}
QLibDiveComputer::~QLibDiveComputer()
{
    delete m_available_devices;
    delete m_available_portnames;
    free_context();
    m_version.clear();
}

QVariant QLibDiveComputer::get_ports_as_qvariant() {
    return QVariant::fromValue(*m_available_portnames);
}

DCComputerList* QLibDiveComputer::get_devices()
{
    DCComputerList* list = new DCComputerList();
    dc_descriptor_t* descriptor;
    dc_iterator_t* iterator;
    dc_status_t status;
    dc_descriptor_iterator(&iterator);

    while((status = dc_iterator_next(iterator, &descriptor)) == DC_STATUS_SUCCESS) {
        list->add(new DCComputer(descriptor));
    }

    return list;
}

void QLibDiveComputer::start_download(QString port_name, DCComputer* descriptor) {
    create_context(port_name.toLatin1().data(), descriptor->descriptor);

    emit start();

    emit progress(1, 5);
    emit progress(2, 5);
    emit progress(3, 5);
    emit progress(4, 5);
    emit progress(5, 5);

    emit done();
}

void QLibDiveComputer::create_context(char *port_name, dc_descriptor_t *descriptor) {
    free_context();

    m_last_context = new DCDownloadContext(this);
    m_last_context->setPortName(port_name);
    m_last_context->setDescriptor(descriptor);
}

void QLibDiveComputer::free_context() {
    if(m_last_context != NULL) {
        delete m_last_context;
        m_last_context = NULL;
    }
}

QStringList* QLibDiveComputer::get_ports() {

    QStringList* dataList = new QStringList();
    QList<QSerialPortInfo> data = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo info : data ) {
       dataList->append(info.portName());
    }

    return dataList;
}


void QLibDiveComputer::get_version()
{
    dc_version_t version;
    dc_version(&version);

    char buff[64];
    sprintf(buff, "v%d.%d.%d", version.major, version.minor, version.micro);
    m_version = buff;
}
