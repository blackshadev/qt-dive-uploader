#include "qdcwritercontroller.h"

QDCWriterController::QDCWriterController(QObject *parent)
    : QThread(parent)
{
    device.model = 0;
    device.serial = 0;
    device.firmware = 0;

    connect(this, &QThread::started, this, [=]() {
        writer->start();
        performAdvance();
    });
}

QDCWriterController::~QDCWriterController()
{
    quit();
    wait();
}

void QDCWriterController::setWriter(QDCWriter *w)
{
    if (writer != NULL) {
        disconnect(writer, NULL, this, NULL);
    }

    writer = w;
    connect(this, &QDCWriterController::advance, this, &QDCWriterController::performAdvance);
    connect(w, SIGNAL(readyForData()), this, SIGNAL(advance()));
    connect(w, &QDCWriter::started, this, [=]() {
        setState(WriterState::Writing);
    });
    connect(w, &QDCWriter::ended, this, [=]() {
        setState(WriterState::Ended);
    });
    connect(w, &QDCWriter::cancelled, this, [=]() {
        setState(WriterState::Cancelled);
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
    if (state > WriterState::Idle) {
        throw std::runtime_error("Already running");
    }
    if (!writer || !device.serial || !descriptor) {
        throw std::runtime_error("Unable to start writing, invalid argument.");
    }

    isEnded = false;

    setCurrent(0);

    writer->setDevice(device);
    writer->setDescriptor(descriptor);

    if(!isRunning()) {
        QThread::start();
    }
    setState(WriterState::Starting);
}

void QDCWriterController::write(DCDive *dive)
{
    mutex.lock();
    queue.enqueue(dive);
    mutex.unlock();
    emit advance();
}

void QDCWriterController::write(QDCDive *dive)
{
    write((DCDive *)dive);
}

void QDCWriterController::end()
{
    isEnded = true;
    emit advance();
}

void QDCWriterController::cancel()
{
    setState(WriterState::Cancelling);
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

bool QDCWriterController::isReadyForData()
{
    return writer->isReadyForData();
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

void QDCWriterController::performAdvance()
{

    mutex.lock();
    switch (state) {
        case WriterState::Idle:
            break;
        case WriterState::Starting:
            writer->start();
        break;

        case WriterState::Writing:
            if (!writer->isReadyForData()) {
                break;
            }

            if (!queue.isEmpty()) {
                DCDive *dive = queue.dequeue();
                process(dive);
                setState(WriterState::Writing);
            }
            if(queue.isEmpty() && isEnded) {
                setState(WriterState::Ending);
            }
            mutex.unlock();
        break;
        case WriterState::Cancelling:
            writer->cancel();
        break;
        case WriterState::Cancelled:
            emit cancelled();
            setState(WriterState::Idle);
        break;
        case WriterState::Ending:
            writer->end();
        break;
        case WriterState::Ended:
            emit finished();
            setState(WriterState::Idle);
        break;
    }
    mutex.unlock();

}

void QDCWriterController::setState(QDCWriterController::WriterState st)
{

    mutex.lock();
    state = st;
    mutex.unlock();
    emit advance();
}

void QDCWriterController::run()
{
    exec();
}
