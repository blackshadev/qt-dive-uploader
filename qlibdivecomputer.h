#ifndef QLIBDIVECOMPUTER_H
#define QLIBDIVECOMPUTER_H
#include "dccomputerlist.h"
#include <QString>
#include <QStringList>
#include <QObject>

class QLibDiveComputer: public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString version MEMBER version CONSTANT)
    Q_PROPERTY(DCComputerList* devices READ get_devices CONSTANT)

public:
    explicit QLibDiveComputer(QObject *parent = 0);
    ~QLibDiveComputer();
    QString version;
    DCComputerList* get_devices();
private:
    void get_version();
};
Q_DECLARE_METATYPE(QLibDiveComputer*)

#endif // QLIBDIVECOMPUTER_H
