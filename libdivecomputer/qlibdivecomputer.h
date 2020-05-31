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

#undef ERROR

class WriteType : public QObject {
    Q_OBJECT
public:
    enum writetype {
        File,
        LittleLog
    };
    Q_ENUM(writetype)

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
    Q_ENUM(loglevel)

};
Q_DECLARE_METATYPE(LogLevel*)

class QLibDiveComputer: public QObject
{

    Q_OBJECT
    Q_PROPERTY(QString version MEMBER m_version CONSTANT)
    Q_PROPERTY(DCComputerList* devices MEMBER m_available_devices CONSTANT)
    Q_PROPERTY(DCTransportList* transports MEMBER m_supported_transports CONSTANT)
    Q_PROPERTY(QStringList LogLevels READ get_logLevels CONSTANT)
    Q_PROPERTY(QString logLevel READ get_logLevel WRITE set_logLevel NOTIFY logLevelChanged)
    Q_PROPERTY(WriteType::writetype writeType READ get_writeType WRITE set_writeType NOTIFY writeTypeChanged)
    Q_PROPERTY(QString path MEMBER m_path NOTIFY pathChanged)
    Q_PROPERTY(bool selectDives MEMBER m_select_dives NOTIFY selectDivesChanged)
    Q_PROPERTY(bool isReady READ get_is_ready NOTIFY isReadyChanged)


public:

    explicit QLibDiveComputer(QObject* parent = 0);
    ~QLibDiveComputer();
    QVariant get_ports_as_qvariant();
    QString m_version;
    QStringList* m_available_portnames;
    DCComputerList* m_available_devices;
    DCTransportList* m_supported_transports;
    QString m_path;
    bool m_select_dives = false;

    bool get_is_ready();
    QString get_writeTypeAsString();
    void set_writeTypeAsString(QString str, WriteType::writetype fallback = WriteType::File);

    void bind_littledivelog(LittleDiveLog* log);

    Q_INVOKABLE void set_available_transports(unsigned int trans);
    Q_INVOKABLE void start_download(QString port_name, int comp_index, bool select = false);
    Q_INVOKABLE void cancel();

signals:
    void writeTypeChanged(WriteType::writetype t);
    void logLevelChanged();
    void pathChanged();
    void selectDivesChanged(bool val);
    void readProgress(uint current, uint total);
    void writeProgress(uint current, uint total);
    void start();
    void done();
    void error(QString msg);
    void log(QString lvl, QString msg);
    void selectDives(DiveWriter* writer, QDiveModel* dives);
    void isReadyChanged();

protected slots:
protected:
    bool m_had_error = false;
    LittleDiveLog* m_log = NULL;
    QString get_logLevel();
    void set_logLevel(QString lvl);
    void set_writeType(WriteType::writetype t);
    WriteType::writetype get_writeType();
private:
    dc_loglevel_t m_loglevel = DC_LOGLEVEL_ERROR;
    WriteType::writetype m_writetype = WriteType::File;
    DCComputerList* get_devices();
    QStringList* get_ports();
    QStringList get_logLevels();
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
