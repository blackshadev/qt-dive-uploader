#ifndef QDCCONTEXT_H
#define QDCCONTEXT_H
#include <QObject>
#include <QList>
#include "../divecomputer/context/dccontext.h"
#include "../descriptor/qdcdescriptor.h"

class QDCContext : public QObject, public DCContext
{
    Q_OBJECT
    Q_PROPERTY(LogLevel loglevel READ getQLogLevel WRITE setQLogLevel NOTIFY loglevelChanged)

public:
    enum LogLevel {
        None,
        Error,
        Warning,
        Info,
        Debug,
        All
    };
    Q_ENUM(LogLevel)

    QDCContext(QObject *parent = NULL);
    virtual ~QDCContext();
    void log(logdata_t logdata) override;

    LogLevel getQLogLevel();

    static loglevel_t translateLogLevel(LogLevel ll);
    static LogLevel translateLogLevel(loglevel_t ll);
    static QString logLevelToString(LogLevel ll);

public slots:
    void setQLogLevel(LogLevel ll);
    void log(QString loglevel, QString msg);

signals:
    void loglevelChanged(LogLevel ll);
    void message(QString loglevel, QString msg);

};
Q_DECLARE_METATYPE(QDCContext *)

#endif // QDCCONTEXT_H
