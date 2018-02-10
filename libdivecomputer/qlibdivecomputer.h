#ifndef QLIBDIVECOMPUTER_H
#define QLIBDIVECOMPUTER_H
#include "dccomputerlist.h"
#include <QString>
#include <QStringList>
#include <QObject>
#include "dcdownloadcontext.h"
#include "dccomputerlist.h"

class QLibDiveComputer: public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString version MEMBER m_version CONSTANT)
    Q_PROPERTY(DCComputerList* devices MEMBER m_available_devices CONSTANT)
    Q_PROPERTY(QVariant ports READ get_ports_as_qvariant CONSTANT)

public:
    explicit QLibDiveComputer(QObject *parent = 0);
    ~QLibDiveComputer();
    QVariant get_ports_as_qvariant();
    QString m_version;
    QStringList* m_available_portnames;
    DCComputerList* m_available_devices;

    Q_INVOKABLE void start_download(QString port_name, DCComputer* descriptor);

signals:
    void dive();
    void progress(uint current, uint total);
    void start();
    void done();

private:
    DCComputerList* get_devices();
    QStringList* get_ports();
    DCDownloadContext* m_last_context;
    void create_context(char* port_name, dc_descriptor_t* descriptor);
    void free_context();
    void get_version();
};
Q_DECLARE_METATYPE(QLibDiveComputer*)

#endif // QLIBDIVECOMPUTER_H
