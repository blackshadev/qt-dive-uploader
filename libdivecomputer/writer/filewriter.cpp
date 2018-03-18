#include "filewriter.h"


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

void FileDiveWriter::write(Dive* dive)
{

    QJsonObject json;

    write_dive(json, dive);

    jsonDives.append(json);

    written(dive);
}

void FileDiveWriter::start() {
    if(file.isOpen()) {
        throw std::runtime_error("File was already opened");
    }
    if (!file.open(QIODevice::WriteOnly)) {
        throw std::runtime_error("Couldn't open save file.");
    }
    JsonDiveWriter::start();
}

void FileDiveWriter::end() {
    if(!file.isOpen()) {
        throw std::runtime_error("File was not yet opened");
    }

    dc_datetime_t dt_now;
    dc_datetime_localtime(&dt_now, dc_datetime_now());
    QJsonObject jsonObject;

    jsonObject["ReadTime"] = QString::fromStdString(format_datetime(&dt_now));
    jsonObject["Dives"] = jsonDives;

    QJsonObject comp;
    write_computer(comp, computer.descr, computer.serial);
    jsonObject["Computer"] = comp;


    QJsonDocument saveDoc(jsonObject);
    file.write(saveDoc.toJson());

    file.close();
    JsonDiveWriter::end();
}
