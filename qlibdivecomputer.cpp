#include "qlibdivecomputer.h"
#include <libdivecomputer/version.h>
#include <QString>

QLibDiveComputer::QLibDiveComputer()
{
}

QString QLibDiveComputer::version;

void QLibDiveComputer::update_version() {
    dc_version_t version;
    dc_version(&version);

    char buff[64];
    sprintf(buff, "v%d.%d.%d", version.major, version.minor, version.micro);
    QLibDiveComputer::version = QString(buff);

}


