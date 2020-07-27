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
    writer = w;
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
    if (running) {
        throw std::runtime_error("Already running");
    }
    if (!writer || !device.serial || !descriptor) {
        throw std::runtime_error("Unable to start writing, invalid argument.");
    }

    writer->setDevice(device);
    writer->setDescriptor(descriptor);

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
    ended = true;
    queueNotEmpty.wakeAll();
}

void QDCWriterController::setMaximum(unsigned int max)
{
    if (maximum == max) {
        return;
    }

    maximum = max;
    emit progress(current, maximum);
}

unsigned int QDCWriterController::getMaximum()
{
    return maximum;
}

void QDCWriterController::setCurrent(unsigned int cur)
{
    if (current == cur) {
        return;
    }

    current = cur;
    emit progress(current, maximum);
}

unsigned int QDCWriterController::getCurrent()
{
    return current;
}

void QDCWriterController::process(DCDive *dive)
{
    writer->write(dive);
    setCurrent(current + 1);
}

void QDCWriterController::run()
{
    writer->start();

    DCDive *dive;
    while (!ended || !queue.isEmpty()) {
        mutex.lock();
        while (queue.isEmpty()) {
            queueNotEmpty.wait(&mutex);
        }
        dive = queue.dequeue();
        mutex.unlock();
        process(dive);
    }

    writer->end();
    emit finished();
}
