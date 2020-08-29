#include "qdccontext.h"
#include <libdivecomputer/version.h>
#include <QVector>

QDCContext::QDCContext(QObject *parent)
    : QObject(parent), DCContext()
{}

QDCContext::~QDCContext()
{}


void QDCContext::log(logdata_t logdata)
{
    DCContext::log(logdata);
    auto ll = translateLogLevel(logdata.loglevel);
    log(
        logLevelToString(ll),
        QString(logdata.msg)
    );
}

void QDCContext::setQLogLevel(QDCContext::LogLevel ll)
{
    loglevel_t _ll = translateLogLevel(ll);
    setLogLevel(_ll);
    emit loglevelChanged(ll);

    QString msg = "Loglevel changed to ";
    msg.append(logLevelToString(ll));
    log(logLevelToString(ll), msg);
}

void QDCContext::log(QString loglevel, QString msg)
{
    emit message(loglevel, msg);
}

QDCContext::LogLevel QDCContext::getQLogLevel()
{
    return translateLogLevel(logLevel);
}

QString QDCContext::getVersion()
{
    dc_version_t version;
    dc_version(&version);

    char buff[64];
    sprintf(buff, "v%d.%d.%d", version.major, version.minor, version.micro);
    return QString::fromLocal8Bit(buff);
}

QDCContext::LogLevel QDCContext::translateLogLevel(loglevel_t ll)
{
    switch (ll) {
        case DC_LOGLEVEL_NONE:
            return LogLevel::None;
        case DC_LOGLEVEL_ERROR:
            return LogLevel::Error;
        case DC_LOGLEVEL_WARNING:
            return LogLevel::Warning;
        case DC_LOGLEVEL_INFO:
            return LogLevel::Info;
        case DC_LOGLEVEL_DEBUG:
            return LogLevel::Debug;
        case DC_LOGLEVEL_ALL:
            return LogLevel::All;
    }
}

QString QDCContext::logLevelToString(LogLevel ll)
{
    switch (ll) {
        case LogLevel::All:
            return "All";
        case LogLevel::None:
            return "None";
        case LogLevel::Info:
            return "Info";
        case LogLevel::Debug:
            return "Debug";
        case LogLevel::Error:
            return "Error";
        case LogLevel::Warning:
            return "Warning";
    }
}

loglevel_t QDCContext::translateLogLevel(QDCContext::LogLevel ll)
{
    switch (ll) {
        case LogLevel::None:
            return DC_LOGLEVEL_NONE;
        case LogLevel::Error:
            return DC_LOGLEVEL_ERROR;
        case LogLevel::Warning:
            return DC_LOGLEVEL_WARNING;
        case LogLevel::Info:
            return DC_LOGLEVEL_INFO;
        case LogLevel::Debug:
            return DC_LOGLEVEL_DEBUG;
        case LogLevel::All:
            return DC_LOGLEVEL_ALL;
    }
}
