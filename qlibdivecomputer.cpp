#include "qlibdivecomputer.h"
#include "dccomputerlist.h"
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

void QLibDiveComputer::get_version()
{
    dc_version_t version;
    dc_version(&version);

    char buff[64];
    sprintf(buff, "v%d.%d.%d", version.major, version.minor, version.micro);
    this->version = buff;

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
