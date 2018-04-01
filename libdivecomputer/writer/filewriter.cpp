#include "filewriter.h"
#include <QTimer>;

FileDiveWriter::FileDiveWriter(QString path)
{
    file.setFileName(path);
}

FileDiveWriter::~FileDiveWriter()
{
    if(file.isOpen()) {
        end();
    }
}

void FileDiveWriter::set_device_descriptor(dc_descriptor_t *descr) {
    computer.descr = descr;
}

void FileDiveWriter::set_device_info(uint model, uint serial, uint firmware) {
    computer.firmware = firmware;
    computer.serial = serial;
}

void FileDiveWriter::do_work(Dive* dive)
{

    QJsonObject json;

    write_dive(json, dive);

    jsonDives.append(json);

    work_done(dive);
}

void FileDiveWriter::do_start() {
    emit starting();
    if(file.isOpen()) {
        throw std::runtime_error("File was already opened");
    }
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Couldn't open save file.");
    }

    m_lock.lock();
    m_ready = true;
    m_lock.unlock();
    emit ready();
}

void FileDiveWriter::do_end() {
    if(!file.isOpen()) {
        qWarning("File was not yet opened");
        _teardown();
        return;
    }

    dc_datetime_t dt_now;
    dc_datetime_localtime(&dt_now, dc_datetime_now());
    QJsonObject jsonObject;

    jsonObject["readtime"] = QString::fromStdString(format_datetime(&dt_now));
    jsonObject["dives"] = jsonDives;

    QJsonObject comp;
    write_computer(comp, computer.descr, computer.serial);
    jsonObject["computer"] = comp;


    QJsonDocument saveDoc(jsonObject);
    file.write(saveDoc.toJson());

    file.close();
    _teardown();
}
