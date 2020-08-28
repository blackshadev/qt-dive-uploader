#include "qdcwritercontroller.h"

QDCWriterController::QDCWriterController(QObject *parent)
    : QObject(parent)
{
    workerThread = new QThread();
    writerWorker = new QDCWriterWorker();
    writerWorker->moveToThread(workerThread);

    connect(writerWorker, &QDCWriterWorker::written, this, [=]() {
        setCurrent(getCurrent() + 1);
    });
    connect(writerWorker, SIGNAL(cancelled()), this, SIGNAL(cancelled()));
    connect(writerWorker, SIGNAL(ended()), this, SIGNAL(ended()));
    connect(writerWorker, SIGNAL(started()), this, SIGNAL(started()));
    connect(writerWorker, SIGNAL(error(QString)), this, SIGNAL(error(QString)));

    workerThread->start();
}

QDCWriterController::~QDCWriterController()
{
    workerThread->quit();
    workerThread->wait();

    delete workerThread;
    delete writerWorker;
}

void QDCWriterController::setWriter(QDCWriteTarget *w)
{
    if (writeTarget) {
        disconnect(w, NULL, this, NULL);
    }

    writeTarget = w;
    writerWorker->setWriter(w);

    if (writeTarget) {
        connect(writeTarget, SIGNAL(isBusyChanged()), this, SIGNAL(isBusyChanged()));
    }
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

bool QDCWriterController::isBusy()
{
    return writeTarget != NULL && writeTarget->isBusy();
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
    setCurrent(0);
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
