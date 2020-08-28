#include "qdcwriterworker.h"

QDCWriterWorker::QDCWriterWorker(QObject* parent) : QObject(parent)
{

    deviceData.model = 0;
    deviceData.serial = 0;
    deviceData.firmware = 0;

    connect(this, SIGNAL(dive(DCDive *)), this, SLOT(receiveDive(DCDive *)));
    connect(this, SIGNAL(start()), this, SLOT(receiveStart()));
    connect(this, SIGNAL(cancel()), this, SLOT(receiveCancel()));
    connect(this, SIGNAL(end()), this, SLOT(receiveEnd()));

}

void QDCWriterWorker::receiveCancel()
{
    isStarted = false;
    writer->cancel();
}

void QDCWriterWorker::receiveStart()
{
    if (isStarted) {
        emit error("Already running");
    }

    writer->setDevice(deviceData);
    writer->setDescriptor(descriptor);

    isEnded = false;
    isStarted = true;
    writer->start();
}

void QDCWriterWorker::receiveEnd()
{
    isEnded = true;
    consume();
}

void QDCWriterWorker::receiveDive(DCDive *dive)
{
    queue.enqueue(dive);
    consume();
}

void QDCWriterWorker::setDevice(DeviceData data)
{
    deviceData = data;
}

void QDCWriterWorker::setDescriptor(QDCDescriptor *descr)
{
    descriptor = descr;
}

void QDCWriterWorker::setWriter(QDCWriteTarget *w)
{
    if (writer) {
        disconnect(writer, NULL, this, NULL);
    }

    writer = w;

    if (!writer) {
        return;
    }

    connect(writer, SIGNAL(ready()), this, SLOT(consume()));
    connect(writer, SIGNAL(ended()), this, SIGNAL(ended()));
    connect(writer, SIGNAL(cancelled()), this, SIGNAL(cancelled()));
    connect(writer, SIGNAL(written()), this, SIGNAL(written()));
    connect(writer, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
}

void QDCWriterWorker::consume()
{
    if (!isStarted || writer->isBusy()) {
        return;
    }

    DCDive *dive;
    if (queue.size()) {
        dive = queue.dequeue();
        writer->write(dive);
    } else if (isEnded) {
        performEnd();
    }

}

void QDCWriterWorker::performEnd()
{
    isStarted = false;
    writer->end();
}
