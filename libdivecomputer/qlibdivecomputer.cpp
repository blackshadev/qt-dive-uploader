#include "qlibdivecomputer.h"
#include "dccomputerlist.h"
#include <QMetaObject>
#include <QMetaEnum>
#include <libdivecomputer/version.h>
#include <libdivecomputer/device.h>
#include <libdivecomputer/descriptor.h>
#include <QSerialPortInfo>
#include "writer/filewriter.h"
#include "writer/littlelogwriter.h"


QLibDiveComputer::QLibDiveComputer(QObject *parent) : QObject(parent)
{
    m_context = NULL;
    m_writer = NULL;
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

QString QLibDiveComputer::get_writeTypeAsString()
{
    auto meta = WriteType::staticMetaObject;
    auto idx = meta.indexOfEnumerator("writetype");
    auto data = meta.enumerator(idx);

    auto key = data.key((int)m_writetype);
    return QString(key);
}

void QLibDiveComputer::set_writeTypeAsString(QString str, WriteType::writetype t)
{
    if(!str.isEmpty()) {
        auto meta = WriteType::staticMetaObject;
        auto idx = meta.indexOfEnumerator("writetype");
        auto data = meta.enumerator(idx);

        t = (WriteType::writetype) data.keyToValue(str.toLocal8Bit().data());
    }

    m_writetype = t;
    emit writeTypeChanged(t);
}


QStringList QLibDiveComputer::get_loglevels() {
    auto meta = LogLevel::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    QStringList list;
    for(int iX = 0; iX < data.keyCount(); iX++) {
        list.append(QString(data.key(iX)));
    }

    return list;
}

QString QLibDiveComputer::get_loglevel() {
    auto meta = LogLevel::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    auto key = data.key((int)m_loglevel);
    return QString(key);
}

void QLibDiveComputer::set_loglevel(QString lvl) {
    auto meta = LogLevel::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    dc_loglevel_t loglevel = (dc_loglevel_t) data.keyToValue(lvl.toLocal8Bit().data());
    m_loglevel = loglevel;
    emit loglevelChanged();
}

WriteType::writetype QLibDiveComputer::get_writeType() {
    return m_writetype;
}

void QLibDiveComputer::set_writeType(WriteType::writetype w) {
    bool isChanged = w != m_writetype;
    m_writetype = w;

    if(isChanged) {
        emit writeTypeChanged(w);
    }
}

void QLibDiveComputer::bind_littledivelog(LittleDiveLog *log)
{
    m_log = log;
}


void QLibDiveComputer::start_download(QString port_name, int comp_idx) {

    auto computer = m_available_devices->get(comp_idx);

    try {
        create_context(port_name.toLatin1().data(), computer->descriptor);
        create_writer();
        m_context->start();

    } catch(std::exception &err) {
        emit error(QString(err.what()));
    }
}

void QLibDiveComputer::create_writer() {
    if(m_writer != NULL) {
        free_writer();
    }

    switch(m_writetype) {
        case WriteType::File:
            m_writer = new FileDiveWriter(m_path);
        break;
        case WriteType::LittleLog:
            m_writer = new LittleLogWriter(m_log);
        break;
        default:
            emit error("No such write type");
            return;
        break;

    }

    connect(m_writer, SIGNAL(finished()), this, SIGNAL(done()));
    connect(m_writer, SIGNAL(progress(uint,uint)), this, SIGNAL(writeProgress(uint,uint)));
    connect(m_writer, &DiveWriter::diveWritten, [](Dive* d) {
        delete d;
    });
}

void QLibDiveComputer::free_writer() {
    if(m_writer != NULL) {
        delete m_writer;
        m_writer = NULL;
    }
}

void QLibDiveComputer::create_context(char *port_name, dc_descriptor_t *descriptor) {
    free_context();

    m_context = new DCDownloadContext(this);
    m_context->setPortName(port_name);
    m_context->setDescriptor(descriptor);
    m_context->setLogLevel(m_loglevel);

    m_context->connect(m_context, &DCDownloadContext::started, this, [=]() {
        m_writer->begin();
        m_writer->set_device_descriptor(descriptor);
        emit start();
    });
    m_context->connect(m_context, SIGNAL(progress(uint,uint)), this, SIGNAL(readProgress(uint,uint)));

    m_context->connect(m_context, &DCDownloadContext::error, this, [=](QString msg) {
        emit error(QString(msg));
    });
    m_context->connect(m_context, SIGNAL(log(QString, QString)), this, SIGNAL(log(QString,QString)));

    m_context->connect(m_context, &DCDownloadContext::deviceInfo, this, [=](uint model, uint serial, uint firmware) {
        m_writer->set_device_info(model, serial, firmware);
    });
    m_context->connect(m_context, &DCDownloadContext::clock, this, [=](uint devtime, uint systime) {
        m_writer->set_device_clock(devtime, systime);
    });

    m_context->connect(m_context, &DCDownloadContext::dive, this, [=](Dive* dive) {
        m_writer->write(dive);
    });

    m_context->connect(m_context, &DCDownloadContext::finished, this, [=]() {
        m_writer->end();
    });

}

void QLibDiveComputer::free_context() {
    if(m_context != NULL) {
        m_context->disconnect();
        delete m_context;
        m_context = NULL;
    }
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
