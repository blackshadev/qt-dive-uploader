#include "qlibdivecomputer.h"
#include "dccomputerlist.h"
#include <libdivecomputer/version.h>
#include <libdivecomputer/device.h>
#include <QSerialPortInfo>


QLibDiveComputer::QLibDiveComputer(QObject *parent) : QObject(parent)
{
    m_context = NULL;
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

    try {
        m_context->start();

    } catch(std::exception &err) {
        emit error(err.what());
    }
}

void QLibDiveComputer::create_context(char *port_name, dc_descriptor_t *descriptor) {
    free_context();

    m_context = new DCDownloadContext(this);
    m_context->setPortName(port_name);
    m_context->setDescriptor(descriptor);
    m_context->connect(m_context, SIGNAL(log(const char*,const char*)), this, SLOT(recvLog(const char*,const char*)));
    m_context->connect(m_context, SIGNAL(progress(uint,uint)), this, SIGNAL(progress(uint,uint)));
    m_context->connect(m_context, &DCDownloadContext::deviceInfo, this, [=](uint model, uint serial, uint firmware) {
        qInfo("model: %u; serial: %u; firmware: %u", model, serial, firmware);
    });
    m_context->connect(m_context, &DCDownloadContext::clock, this, [=](uint devtime, uint systime) {
        qInfo("devtime: %u; systime: %u", devtime, systime);
    });
}

void QLibDiveComputer::free_context() {
    if(m_context != NULL) {
        m_context->disconnect();
        delete m_context;
        m_context = NULL;
    }
}

void QLibDiveComputer::recvLog(const char *lvl, const char *msg) {
    emit log(QString(lvl), QString(msg));
}

QStringList* QLibDiveComputer::get_ports() {

    QStringList* dataList = new QStringList();
    QList<QSerialPortInfo> data = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo info : data ) {
       dataList->append(info.systemLocation());
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
