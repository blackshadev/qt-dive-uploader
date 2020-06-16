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
    m_available_ports = new DCPortList(this);
    m_supported_transports = new DCTransportList(this);
    m_supported_transports->loadTransports(DC_TRANSPORT_BLE|DC_TRANSPORT_USBHID|DC_TRANSPORT_IRDA);
    get_version();
    connect(this, SIGNAL(pathChanged()), this, SIGNAL(isReadyChanged()));
    connect(m_available_ports, SIGNAL(loadend()), this, SIGNAL(availablePortsChanged()));
}
QLibDiveComputer::~QLibDiveComputer()
{
    delete m_available_devices;
    delete m_available_ports;
    delete m_supported_transports;

    free_context();
    free_writer();
    m_version.clear();
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

void QLibDiveComputer::set_available_transports(unsigned int trans)
{
    m_supported_transports->loadTransports(trans);
    emit transportChanged();
}


void QLibDiveComputer::update_availble_ports(int computer_idx, unsigned int trans)
{
    dc_context_t* ctx;
    dc_context_new(&ctx);
    dc_context_set_loglevel(ctx, m_loglevel);
    dc_context_set_logfunc(ctx, QLibDiveComputer::dc_log, NULL );

    auto computer = m_available_devices->get(computer_idx);
    m_available_ports->load(ctx, computer->descriptor, (dc_transport_t)trans);

    dc_context_free(ctx);

    emit availablePortsChanged();
}

void QLibDiveComputer::dc_log(dc_context_t *context, dc_loglevel_t loglevel, const char *file, unsigned int line, const char *function, const char *msg, void *userdata)
{
    auto lib = (QLibDiveComputer*)userdata;
    const char *loglevels[] = {"NONE", "ERROR", "WARNING", "INFO", "DEBUG", "ALL"};
    emit lib->log(QString(loglevels[loglevel]), QString(msg));
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


QStringList QLibDiveComputer::get_logLevels() {
    auto meta = LogLevel::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    QStringList list;
    for(int iX = 0; iX < data.keyCount(); iX++) {
        list.append(QString(data.key(iX)));
    }

    return list;
}

bool QLibDiveComputer::get_is_ready() {
    switch(m_writetype) {
        case WriteType::File:
            return !m_path.isEmpty();
        case WriteType::LittleLog:
            return m_log != NULL && m_log->isLoggedIn() && m_log->m_user_info != NULL;
        default:
            return false;
    }
}

QString QLibDiveComputer::get_logLevel() {
    auto meta = LogLevel::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    auto key = data.key((int)m_loglevel);
    return QString(key);
}

void QLibDiveComputer::set_logLevel(QString lvl) {
    auto meta = LogLevel::staticMetaObject;
    auto idx = meta.indexOfEnumerator("loglevel");
    auto data = meta.enumerator(idx);

    dc_loglevel_t loglevel = (dc_loglevel_t) data.keyToValue(lvl.toLocal8Bit().data());
    m_loglevel = loglevel;
    emit logLevelChanged();
}

WriteType::writetype QLibDiveComputer::get_writeType() {
    return m_writetype;
}

void QLibDiveComputer::set_writeType(WriteType::writetype w) {
    bool isChanged = w != m_writetype;
    m_writetype = w;

    if(isChanged) {
        emit writeTypeChanged(w);
        emit isReadyChanged();
    }
}

void QLibDiveComputer::bind_littledivelog(LittleDiveLog *log)
{
    m_log = log;
    connect(log, SIGNAL(userInfoChanged(UserInfo*)), this, SIGNAL(isReadyChanged()));
    connect(log, SIGNAL(loggedStateChanged(bool)), this, SIGNAL(isReadyChanged()));
}


void QLibDiveComputer::cancel() {
    m_is_cancelling = true;
    if(m_context != NULL) {
        m_context->cancel();
        m_context->wait();
        delete m_context;
        m_context = NULL;
    }

    if(m_writer != NULL) {
        free_writer();
    }
}

void QLibDiveComputer::start_download(int port_idx, int comp_idx, bool select_dives) {

    if(m_writetype == WriteType::LittleLog && m_log->m_user_info == NULL) {
        emit error("Not yet logged in. Either login or wait a second");
        return;
    }

    auto computer = m_available_devices->get(comp_idx);

    auto port = m_available_ports->get(port_idx);

    try {
        create_download_context(port, computer);
        create_writer(computer->descriptor, select_dives);
        m_context->start();

    } catch(std::exception &err) {
        emit error(QString(err.what()));
    }
}

void QLibDiveComputer::create_writer(dc_descriptor_t* descr, bool select_dives) {
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

    m_writer->set_selection(select_dives);

    m_writer->set_device_descriptor(descr);

    connect(m_writer, SIGNAL(done()), this, SIGNAL(done()));
    connect(m_writer, SIGNAL(progress(uint,uint)), this, SIGNAL(writeProgress(uint,uint)));
    connect(m_writer, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    connect(m_writer, &DiveWriter::selectDives, [&](DiveWriter* writer, QList<Dive*> dives) {
        auto model = new QDiveModel();
        for(auto d : dives) {
            model->add(d);
        }

        emit selectDives(writer, model);
    });
    connect(m_writer, &DiveWriter::diveWritten, [](Dive* d) {});
}

void QLibDiveComputer::free_writer() {
    if(m_writer != NULL) {
        m_writer->disconnect();
        m_writer->quit();
        m_writer->wait();
        delete m_writer;
        m_writer = NULL;
    }
}

void QLibDiveComputer::create_download_context(DCPort* port, DCComputer* comp) {
    free_context();

    m_had_error = false;
    m_context = new DCDownloadContext(this);
    m_context->setComputer(comp);
    m_context->setPort(port);
    m_context->setLogLevel(m_loglevel);

    m_context->connect(m_context, &DCDownloadContext::started, this, [&]() {
        emit start();
    });
    m_context->connect(m_context, SIGNAL(progress(uint,uint)), this, SIGNAL(readProgress(uint,uint)));

    m_context->connect(m_context, &DCDownloadContext::error, [&](QString err) {
        m_had_error = true;
        emit error(err);
    });
    m_context->connect(m_context, SIGNAL(log(QString, QString)), this, SIGNAL(log(QString,QString)));

    m_context->connect(m_context, &DCDownloadContext::deviceInfo, this, [=](uint model, uint serial, uint firmware) {
        if(m_log->m_user_info != NULL) {
            auto comp_data = m_log->m_user_info->get_computer(serial);
            if(comp_data != NULL) {
                auto fingerprint_data = (const unsigned char*)comp_data->fingerprint.data();
                m_context->setFingerprint(fingerprint_data, (unsigned int)comp_data->fingerprint.size());
            }
        }
        m_writer->set_device_info(model, serial, firmware);
        m_writer->start();
    });
    m_context->connect(m_context, &DCDownloadContext::clock, this, [=](uint devtime, uint systime) {
        m_writer->set_device_clock(devtime, systime);
    });

    m_context->connect(m_context, &DCDownloadContext::dive, this, [=](Dive* dive) {
        if(m_is_cancelling) {
            return;
        }
        m_writer->add(dive);
    });

    m_context->connect(m_context, &DCDownloadContext::finished, this, [&]() {
        if(!m_had_error && !m_is_cancelling) {
            m_writer->end();
            m_log->fetch_user_data();
        } else {
            emit done();
        }
    });

}

void QLibDiveComputer::free_context() {
    if(m_context != NULL) {
        m_context->wait();
        m_context->disconnect();
        delete m_context;
        m_context = NULL;
    }
}


void QLibDiveComputer::get_version()
{
    dc_version_t version;
    dc_version(&version);

    char buff[64];
    sprintf(buff, "v%d.%d.%d", version.major, version.minor, version.micro);
    m_version = QString::fromLocal8Bit(buff);
}
