#ifndef QLIBDIVECOMPUTER_H
#define QLIBDIVECOMPUTER_H
#include "dccomputerlist.h"
#include <QString>
#include <QStringList>

class QLibDiveComputer
{
public:
    QLibDiveComputer();
    ~QLibDiveComputer();
    QString version;
    DCComputerList* get_devices();
private:
    void get_version();
};

#endif // QLIBDIVECOMPUTER_H
