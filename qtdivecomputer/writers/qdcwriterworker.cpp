#include "qdcwriterworker.h"

QDCWriterWorker::QDCWriterWorker(QObject* parent) : QObject(parent)
{

    deviceData.model = 0;
    deviceData.serial = 0;
    deviceData.firmware = 0;

}

void QDCWriterWorker::receiveCancel()
{
    writer->cancel();

}

void QDCWriterWorker::receiveStart()
{
    if (isStarted) {
        emit error("Already running");
    }

    writer->setDevice(deviceData);
    writer->setDescriptor(descriptor);

    isStarted = true;
    writer->start();
}

void QDCWriterWorker::receiveEnd()
{
    isEnded = true;
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

void QDCWriterWorker::setWriter(QDCWriter *w)
{
    if (writer) {
        disconnect(writer, NULL, this, NULL);
    }

    writer = w;

    connect(writer, &QDCWriter::ready, this, &QDCWriterWorker::consume);
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
    } else if (isEnded) {
        isStarted = false;
        emit ended();
    }
}
