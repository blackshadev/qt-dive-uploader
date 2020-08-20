#include "qdcwritercontroller.h"

QDCWriterController::QDCWriterController(QObject *parent)
    : QObject(parent)
{
    device.model = 0;
    device.serial = 0;
    device.firmware = 0;

    workerThread = new QThread();
    workerThread->start();
}

QDCWriterController::~QDCWriterController()
{
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
}

void QDCWriterController::setWriter(QDCWriter *w)
{
    if (writer) {
        disconnect(writer, NULL, this, NULL);
    }


    writer = w;
    w->moveToThread(workerThread);

    connect(writer, &QDCWriter::written, this, [=]() {
        setCurrent(getCurrent() + 1);
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
    emit writer->doWrite((QDCDive *)dive);
}

void QDCWriterController::write(DCDive *dive)
{
    write((QDCDive *)dive);
}

void QDCWriterController::start()
{
    writer->setDevice(device);
    writer->setDescriptor(descriptor);
    emit writer->doStart();
}

void QDCWriterController::end()
{
    emit writer->doEnd();
}

void QDCWriterController::cancel()
{
    emit writer->doCancel();
}
