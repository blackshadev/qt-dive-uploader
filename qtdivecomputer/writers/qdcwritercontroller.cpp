#include "qdcwritercontroller.h"

QDCWriterController::QDCWriterController(QObject *parent)
    : QThread(parent)
{
    device.model = 0;
    device.serial = 0;
    device.firmware = 0;

    connect(this, SIGNAL(moreWork(QPivateSignal)), this, SLOT(tryProcessWork()));

}

QDCWriterController::~QDCWriterController()
{
    quit();
    wait();
}

void QDCWriterController::setWriter(QDCWriter *w)
{
    writer = w;

    disconnect(w, NULL, this, NULL);
    w->moveToThread(this);

    connect(w, SIGNAL(ready()), this, SIGNAL(moreWork(QPrivateSignal)));

    connect(this, SIGNAL(starting(QPrivateSignal)), writer, SLOT(start()));
    connect(this, SIGNAL(ending(QPrivateSignal)), writer, SLOT(end()));
    connect(this, SIGNAL(writing(DCDive *, QPrivateSignal)), writer, SLOT(write(DCDive *)));
    connect(writer, &QDCWriter::written, this, [=]() {
        //
    });


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
    setCurrent(0);

    writer->setDevice(device);
    writer->setDescriptor(descriptor);

    if(!isRunning()) {
        QThread::start();
    }

    emit starting(QPrivateSignal());
}

void QDCWriterController::write(DCDive *dive)
{
    mutex.lock();
    queue.enqueue(dive);
    mutex.unlock();

    emit moreWork(QPrivateSignal());
}

void QDCWriterController::write(QDCDive *dive)
{
    write((DCDive *)dive);
}

void QDCWriterController::end()
{
    isEnding = true;
}

void QDCWriterController::cancel()
{
    writer->cancel();
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
    exec();
}

void QDCWriterController::tryProcessWork()
{
    if (!writer) {
        throw std::runtime_error("No writer set");
    }

    if (writer->isBusy()) {
        return;
    }

    mutex.lock();
    if(queue.length() > 0) {
        DCDive *d = queue.dequeue();
        mutex.unlock();

        emit writing(d, QPrivateSignal());
    } else if(isEnding) {
        // done
        mutex.unlock();

        emit ending(QPrivateSignal());
    } else {
        mutex.unlock();
    }


}
