#include "qdcwritercontroller.h"

QDCWriterController::QDCWriterController(QObject *parent)
    : QThread(parent)
{
    device.model = 0;
    device.serial = 0;
    device.firmware = 0;
}

QDCWriterController::~QDCWriterController()
{

}

void QDCWriterController::setWriter(QDCWriter *w)
{
    if (writer != NULL) {
        disconnect(writer, NULL, this, NULL);
    }

    writer = w;
    connect(w, &QDCWriter::isWriteReadyChanged, this, [=]() {
        if (!isWriteReady()) {
            return;
        }

        mutex.lock();
        readyForNextWrite.wakeAll();
        mutex.unlock();
    });
    connect(w, &QDCWriter::written, this, [=]() {
        setCurrent(getCurrent() + 1);
    });

    connect(w, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
}

void QDCWriterController::setDevice(QDeviceData dev)
{
    setDevice((DeviceData)dev);
}

void QDCWriterController::setDevice(DeviceData dev)
{
    device = dev;
}

void QDCWriterController::setDescriptor(DCDeviceDescriptor *descr)
{
    setDescriptor((QDCDescriptor *)descr);
}

void QDCWriterController::setDescriptor(QDCDescriptor *descr)
{
    descriptor = descr;
}

void QDCWriterController::start()
{
    if (isBusy) {
        throw std::runtime_error("Already running");
    }
    if (!writer || !device.serial || !descriptor) {
        throw std::runtime_error("Unable to start writing, invalid argument.");
    }

    isCancelled = false;
    isEnded = false;

    setCurrent(0);

    writer->setDevice(device);
    writer->setDescriptor(descriptor);

    emit started();
    QThread::start();
}

void QDCWriterController::write(DCDive *dive)
{
    mutex.lock();
    queue.enqueue(dive);
    queueNotEmpty.wakeAll();
    mutex.unlock();
}

void QDCWriterController::write(QDCDive *dive)
{
    write((DCDive *)dive);
}

void QDCWriterController::end()
{
    isEnded = true;
    queueNotEmpty.wakeAll();
    readyForNextWrite.wakeAll();
}

void QDCWriterController::cancel()
{
    isCancelled = true;
    queueNotEmpty.wakeAll();
    readyForNextWrite.wakeAll();
}

bool QDCWriterController::getBusy()
{
    return isBusy;
}

void QDCWriterController::setBusy(bool b)
{
    if (isBusy == b) {
        return;
    }

    isBusy = b;
    emit isBusyChanged();
}

bool QDCWriterController::isWriteReady()
{
    return writer->isWriteReady();
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

void QDCWriterController::process(DCDive *dive)
{
    writer->write(dive);
}

void QDCWriterController::run()
{
    writer->start();

    DCDive *dive;
    while (!isEnded || !queue.isEmpty()) {
        mutex.lock();
        waitForQueueNotEmpty(&mutex);
        waitForWriteReady(&mutex);
        if (isCancelled) {
            break;
        }

        dive = queue.dequeue();
        mutex.unlock();
        process(dive);
    }

    if (isCancelled) {
        mutex.unlock();
        emit cancelled();
        return;
    }

    writer->end();
    emit finished();
}

void QDCWriterController::waitForWriteReady(QMutex *mutex)
{
    while (!isCancelled && !isWriteReady()) {
        readyForNextWrite.wait(mutex);
    }
}

void QDCWriterController::waitForQueueNotEmpty(QMutex *mutex)
{
    while (!isCancelled && queue.isEmpty()) {
        queueNotEmpty.wait(mutex);
    }
}
