#include "qdcwritercontroller.h"

QDCWriterController::QDCWriterController(QObject *parent)
    : QObject(parent)
{
    writerWorker = new QDCWriterWorker();
    workerThread = new QThread();
    workerThread->start();
    writerWorker->moveToThread(workerThread);

    connect(writerWorker, &QDCWriterWorker::written, this, [=]() {
        setCurrent(getCurrent() + 1);
    });
    connect(writerWorker, SIGNAL(cancelled()), this, SIGNAL(cancelled()));
    connect(writerWorker, SIGNAL(ended()), this, SIGNAL(ended()));
    connect(writerWorker, SIGNAL(started()), this, SIGNAL(started()));
    connect(writerWorker, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
}

QDCWriterController::~QDCWriterController()
{
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
    delete writerWorker;
}

void QDCWriterController::setWriter(QDCWriter *w)
{
    writerWorker->setWriter(w);
    w->moveToThread(workerThread);
}

void QDCWriterController::setDevice(QDeviceData dev)
{
    setDevice((DeviceData)dev);
}

void QDCWriterController::setDevice(DeviceData dev)
{
    writerWorker->setDevice(dev);
}

void QDCWriterController::setDescriptor(DCDeviceDescriptor *descr)
{
    setDescriptor((QDCDescriptor *)descr);
}

void QDCWriterController::setDescriptor(QDCDescriptor *descr)
{
    writerWorker->setDescriptor(descr);
}

unsigned int QDCWriterController::getMaximum()
{
    return maximum;
}

void QDCWriterController::setMaximum(unsigned int max)
{
    if (maximum == max) {
        return;
    }

    maximum = max;
}

unsigned int QDCWriterController::getCurrent()
{
    return current;
}

void QDCWriterController::setCurrent(unsigned int cur)
{
    if(cur == current) {
        return;
    }

    current = cur;
    emit progress(current, maximum);
}

void QDCWriterController::write(QDCDive *dive)
{
    emit writerWorker->dive(dive);
}

void QDCWriterController::write(DCDive *dive)
{
    write((QDCDive *)dive);
}

void QDCWriterController::start()
{
    emit writerWorker->start();
}

void QDCWriterController::end()
{
    emit writerWorker->end();
}

void QDCWriterController::cancel()
{
    emit writerWorker->cancel();
}
