#ifndef QLIBDIVECOMPUTER_H
#define QLIBDIVECOMPUTER_H
#include <QString>

class QLibDiveComputer
{
public:
    QLibDiveComputer();
    static QString version;
    static void update_version();
};

#endif // QLIBDIVECOMPUTER_H
