#include "qlibdivecomputer.h"
#include <libdivecomputer/version.h>
#include <libdivecomputer/device.h>


QLibDiveComputer::QLibDiveComputer()
{
    this->version = QString();
    this->get_version();
}
QLibDiveComputer::~QLibDiveComputer()
{
    this->version.clear();
}

void QLibDiveComputer::get_version() {
    dc_version_t version;
    dc_version(&version);

    char buff[64];
    sprintf(buff, "v%d.%d.%d", version.major, version.minor, version.micro);
    this->version = buff;

}

QStringList* QLibDiveComputer::get_devices() {
    QStringList* list = new QStringList();
    dc_descriptor_t* descriptor;
    dc_iterator_t* iterator;
    dc_status_t status;
    dc_descriptor_iterator(&iterator);

    while((status = dc_iterator_next(iterator, &descriptor)) == DC_STATUS_SUCCESS) {
        QString s = QString();
        s += dc_descriptor_get_vendor(descriptor);
        s += ' ';
        s += dc_descriptor_get_product(descriptor);
        list->append(s);
    }
    list->sort();
    return list;
}

