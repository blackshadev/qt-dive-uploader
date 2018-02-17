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
    Q_PROPERTY(QStringList loglevels READ get_loglevels CONSTANT)
    Q_PROPERTY(QString loglevel READ get_loglevel WRITE set_loglevel NOTIFY loglevelChanged)


public:
    enum loglevel {
        NONE = DC_LOGLEVEL_NONE,
        ERROR = DC_LOGLEVEL_ERROR,
        WARNING = DC_LOGLEVEL_WARNING,
        INFO = DC_LOGLEVEL_INFO,
        DEBUG = DC_LOGLEVEL_DEBUG,
        ALL = DC_LOGLEVEL_ALL
    };
    Q_ENUMS(loglevel)


    explicit QLibDiveComputer(QObject* parent = 0);
    ~QLibDiveComputer();
    QVariant get_ports_as_qvariant();
    QString m_version;
    QStringList* m_available_portnames;
    DCComputerList* m_available_devices;

    Q_INVOKABLE void start_download(QString port_name, int comp_index);

signals:
    void loglevelChanged();
    void dive();
    void progress(uint current, uint total);
    void start();
    void done();
    void error(QString msg);
    void log(QString lvl, QString msg);
protected slots:
    void recvLog(const char* lvl, const char* msg);
    QString get_loglevel();
    void set_loglevel(QString lvl);
private:
    dc_loglevel_t m_loglevel = DC_LOGLEVEL_ERROR;
    DCComputerList* get_devices();
    QStringList* get_ports();
    QStringList get_loglevels();
    DCDownloadContext* m_context;
    void create_context(char* port_name, dc_descriptor_t* descriptor);
    void free_context();
    void get_version();

};
Q_DECLARE_METATYPE(QLibDiveComputer*)

#endif // QLIBDIVECOMPUTER_H
