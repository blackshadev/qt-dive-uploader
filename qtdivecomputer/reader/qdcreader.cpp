#include "qdcreader.h"

QDCReader::QDCReader(QObject *parent) : QObject(parent), DCReader()
{}

QDCReader *QDCReader::setDevice(DCDeviceInterface *d)
{
    DCReader::setDevice(d);
    return this;
}

QDCReader *QDCReader::setParser(DCDiveParserInterface *p)
{
    DCReader::setParser(p);
    return this;
}

void QDCReader::start()
{

    qInfo() << "QDCReader::start " << QThread::currentThreadId();
    DCReader::start();
}

void QDCReader::setFingerprint(fingerprint_t data)
{
    DCReader::setFingerprint(data);
}

void QDCReader::cancel()
{
    DCReader::cancel();
}

void QDCReader::receiveError(const char *msg)
{
//    emit error(QString(msg));
}

void QDCReader::receiveDeviceInfoEvent(dc_event_devinfo_t *devInfo)
{
//    emit deviceInfo(devInfo->model, devInfo->serial, devInfo->firmware);
}

void QDCReader::receiveProgressEvent(dc_event_progress_t *data)
{
    qInfo() << data->current;
    emit progress(data->current, data->maximum);
}

void QDCReader::receiveWaitingEvent()
{
//    emit waiting();
}

void QDCReader::receiveClockEvent(dc_event_clock_t *data)
{
//    emit clock(data->devtime, data->systime);
}

void QDCReader::receiveDive(DCDive *data)
{
//    emit dive((QDCDive *)data);
}
