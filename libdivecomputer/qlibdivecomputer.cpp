#include "qlibdivecomputer.h"
#include "dccomputerlist.h"
#include <QMetaObject>
#include <QMetaEnum>
#include <libdivecomputer/version.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>
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
        list->addComputer(descriptor);
    }

    dc_iterator_free(iterator);

    return list;
}

QStringList QLibDiveComputer::get_loglevels() {
    qInfo("Get loglevels");
    auto meta = QLibDiveComputer::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    QStringList list;
    for(int iX = 0; iX < data.keyCount(); iX++) {
        list.append(QString(data.key(iX)));
    }


    return list;
}

QString QLibDiveComputer::get_loglevel() {
    auto meta = QLibDiveComputer::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    auto key = data.key((int)m_loglevel);
    return QString(key);
}

void QLibDiveComputer::set_loglevel(QString lvl) {
    auto meta = QLibDiveComputer::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    dc_loglevel_t loglevel = (dc_loglevel_t) data.keyToValue(lvl.toLocal8Bit().data());
    m_loglevel = loglevel;
    emit loglevelChanged();

}

void QLibDiveComputer::start_download(QString port_name, int comp_idx) {

    auto computer = m_available_devices->get(comp_idx);

    create_context(port_name.toLatin1().data(), computer->descriptor);

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
    m_context->setLogLevel(m_loglevel);
    m_context->connect(m_context, SIGNAL(started()), this, SIGNAL(start()));
    m_context->connect(m_context, SIGNAL(finished()), this, SIGNAL(done()));
    m_context->connect(m_context, SIGNAL(progress(uint,uint)), this, SIGNAL(progress(uint,uint)));

    m_context->connect(m_context, SIGNAL(log(const char*,const char*)), this, SLOT(recvLog(const char*,const char*)));

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
    m_version = QString::fromLocal8Bit(buff);
}
