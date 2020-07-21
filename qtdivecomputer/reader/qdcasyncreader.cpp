#include "qdcasyncreader.h"

QDCAsyncReader::QDCAsyncReader()
{
    reader = new QDCReader;
    reader->moveToThread(&workerThread);
    connect(&workerThread, &QThread::finished, reader, &QObject::deleteLater);
    connect(&workerThread, &QThread::started, reader, &QDCReader::start);
    connect(reader, &QDCReader::progress, this, &QDCAsyncReader::progress);
    connect(reader, &QDCReader::clock, this, &QDCAsyncReader::clock);
    connect(reader, &QDCReader::progress, this, &QDCAsyncReader::progress);
    connect(reader, &QDCReader::deviceInfo, this, &QDCAsyncReader::deviceInfo);
    connect(reader, &QDCReader::waiting, this, &QDCAsyncReader::waiting);
    connect(reader, &QDCReader::dive, this, &QDCAsyncReader::dive);
    connect(reader, SIGNAL(QDCReader::error(QString)), this, SIGNAL(QDCAsyncReader::error(QString)));
}

QDCAsyncReader::~QDCAsyncReader()
{
    workerThread.quit();
    workerThread.wait();
}

DCReaderInterface *QDCAsyncReader::setDevice(DCDeviceInterface *d)
{
    device = d;
    return this;
}

DCReaderInterface *QDCAsyncReader::setParser(DCDiveParserInterface *p)
{
    parser = p;
    return this;
}

void QDCAsyncReader::start()
{
    reader->setDevice(device);
    reader->setParser(parser);
    workerThread.start();

}
