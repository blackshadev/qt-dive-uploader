#include "qdccontext.h"
#include <QVector>

QDCContext::QDCContext(QObject *parent)
    : QObject(parent), DCContext()
{
    setLogFunction([this](logdata_t data) {
        QString loglevel = logLevelToString(translateLogLevel(data.loglevel));
        QString msg = data.msg;

        emit log(loglevel, msg);
    });
}

QDCContext::~QDCContext()
{}

DCDeviceDescriptor *QDCContext::createDescriptor(dc_descriptor_t *descr)
{
    return new QDCDescriptor(descr, this);
}

void QDCContext::setQLogLevel(QDCContext::LogLevel ll)
{
    loglevel_t _ll = translateLogLevel(ll);
    setLogLevel(_ll);
    emit loglevelChanged(ll);
    emit log(logLevelToString(ll), "Loglevel changed");
}

QDCContext::LogLevel QDCContext::getQLogLevel()
{
    return translateLogLevel(logLevel);
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
