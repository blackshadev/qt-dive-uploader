#ifndef QLIBDIVECOMPUTER_H
#define QLIBDIVECOMPUTER_H
#include "dccomputerlist.h"
#include <QString>
#include <QStringList>
#include <QObject>
#include "dcdownloadcontext.h"
#include "dccomputerlist.h"
#include "qdivemodel.h"
#include "writer/divewriter.h"
#include "../littledivelog/littledivelog.h"
#include "qdivemodel.h"

class WriteType : public QObject {
    Q_OBJECT
public:
    enum writetype {
        File,
        LittleLog
    };
    Q_ENUMS(writetype)

};


class LogLevel: public QObject {
    Q_OBJECT
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

};
Q_DECLARE_METATYPE(LogLevel*)

class QLibDiveComputer: public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString version MEMBER m_version CONSTANT)
    Q_PROPERTY(DCComputerList* devices MEMBER m_available_devices CONSTANT)
    Q_PROPERTY(QVariant ports READ get_ports_as_qvariant CONSTANT)
    Q_PROPERTY(QStringList loglevels READ get_loglevels CONSTANT)
    Q_PROPERTY(QString loglevel READ get_loglevel WRITE set_loglevel NOTIFY loglevelChanged)
    Q_PROPERTY(WriteType::writetype writeType READ get_writeType WRITE set_writeType NOTIFY writeTypeChanged)
    Q_PROPERTY(QString path MEMBER m_path NOTIFY pathChanged)
    Q_PROPERTY(bool selectDives MEMBER m_select_dives NOTIFY selectDivesChanged)

public:

    explicit QLibDiveComputer(QObject* parent = 0);
    ~QLibDiveComputer();
    QVariant get_ports_as_qvariant();
    QString m_version;
    QStringList* m_available_portnames;
    DCComputerList* m_available_devices;
    QString m_path;
    bool m_select_dives = false;

    QString get_writeTypeAsString();
    void set_writeTypeAsString(QString str, WriteType::writetype fallback = WriteType::File);

    void bind_littledivelog(LittleDiveLog* log);

    Q_INVOKABLE void start_download(QString port_name, int comp_index, bool select = false);
    Q_INVOKABLE void cancel();

signals:
    void writeTypeChanged(WriteType::writetype t);
    void loglevelChanged();
    void pathChanged();
    void selectDivesChanged(bool val);
    void readProgress(uint current, uint total);
    void writeProgress(uint current, uint total);
    void start();
    void done();
    void error(QString msg);
    void log(QString lvl, QString msg);
    void selectDives(DiveWriter* writer, QDiveModel* dives);

protected slots:
protected:
    LittleDiveLog* m_log = NULL;
    QString get_loglevel();
    void set_loglevel(QString lvl);
    void set_writeType(WriteType::writetype t);
    WriteType::writetype get_writeType();
private:
    dc_loglevel_t m_loglevel = DC_LOGLEVEL_ERROR;
    WriteType::writetype m_writetype = WriteType::File;
    DCComputerList* get_devices();
    QStringList* get_ports();
    QStringList get_loglevels();
    DCDownloadContext* m_context;
    DiveWriter* m_writer;
    void create_writer(dc_descriptor_t* descriptor, bool select);
    void free_writer();
    void create_context(char* port_name, dc_descriptor_t* descriptor);
    void free_context();
    void get_version();

};
Q_DECLARE_METATYPE(QLibDiveComputer*)

#endif // QLIBDIVECOMPUTER_H
