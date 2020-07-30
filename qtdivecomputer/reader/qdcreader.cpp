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

QDCReader *QDCReader::setContext(DCContextInterface *ctx)
{
    DCReader::setContext(ctx);
    return this;
}

void QDCReader::startReading()
{
    if (isBusy) {
        throw std::runtime_error("Already reading");
    }

    setIsBusy(true);
    DCReader::startReading();
}

void QDCReader::setFingerprint(QByteArray data)
{
    fingerprint_t fp;
    fp.data = (unsigned char *)data.data();
    fp.size = data.size();

    setFingerprint(fp);
}

void QDCReader::setIsBusy(bool b)
{
    if (isBusy == b) {
        return;
    }
    isBusy = b;
    emit isBusyChanged();
}

void QDCReader::setFingerprint(fingerprint_t fp)
{
    DCReader::setFingerprint(fp);
}

bool QDCReader::getIsBusy()
{
    return isBusy;
}

void QDCReader::cancel()
{
    DCReader::cancel();
}

void QDCReader::receiveError(const char *msg)
{
    emit error(QString(msg));
}

void QDCReader::receiveDeviceInfoEvent(dc_event_devinfo_t *devInfo)
{
    QDeviceData devData;
    devData.model = devInfo->model;
    devData.serial = devInfo->serial;
    devData.firmware = devInfo->firmware;

    emit deviceInfo(devData);
}

void QDCReader::receiveProgressEvent(dc_event_progress_t *data)
{
    emit progress(data->current, data->maximum);
}

void QDCReader::receiveWaitingEvent()
{
    emit waiting();
}

void QDCReader::receiveClockEvent(dc_event_clock_t *data)
{
    emit clock(data->devtime, data->systime);
}

void QDCReader::receiveDive(DCDive *data)
{
    emit dive((QDCDive *)data);
}

void QDCReader::receiveFinished()
{
    emit finished();
    setIsBusy(false);
}

void QDCReader::receiveCancelled()
{
    emit cancelled();
    setIsBusy(false);
}
