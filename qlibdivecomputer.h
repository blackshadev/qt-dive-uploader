#ifndef QLIBDIVECOMPUTER_H
#define QLIBDIVECOMPUTER_H
#include <QString>
#include <QStringList>

class QLibDiveComputer
{
public:
    QLibDiveComputer();
    ~QLibDiveComputer();
    QString version;
    QStringList* get_devices();
private:
    void get_version();
};

#endif // QLIBDIVECOMPUTER_H
