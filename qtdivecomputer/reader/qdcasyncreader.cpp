#include "qdcasyncreader.h"

QDCAsyncReader::QDCAsyncReader()
{

    qInfo() << "Construct " << QThread::currentThreadId();
    workerThread = new QThread;
    reader = new QDCReader;
    reader->moveToThread(workerThread);
    reader->connect(this, SIGNAL(startWork()), reader, SLOT(start()));
    reader->connect(reader, SIGNAL(progress(unsigned int, unsigned int)), this, SIGNAL(progress(unsigned int, unsigned int)));
    reader->connect(reader, &QDCReader::clock, this, &QDCAsyncReader::clock);
    reader->connect(reader, &QDCReader::progress, this, &QDCAsyncReader::progress);
    reader->connect(reader, &QDCReader::deviceInfo, this, &QDCAsyncReader::deviceInfo);
    reader->connect(reader, &QDCReader::waiting, this, &QDCAsyncReader::waiting);
    reader->connect(reader, &QDCReader::dive, this, &QDCAsyncReader::dive);
    reader->connect(reader, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    reader->moveToThread(workerThread);
    workerThread->start();
}

QDCAsyncReader::~QDCAsyncReader()
{
    workerThread->quit();
    workerThread->wait();
    delete workerThread;
    delete reader;
}

DCReaderInterface *QDCAsyncReader::setDevice(DCDeviceInterface *d)
{
    device = d;
    reader->setDevice(d);
    return this;
}

DCReaderInterface *QDCAsyncReader::setParser(DCDiveParserInterface *p)
{
    parser = p;
    reader->setParser(parser);
    return this;
}

void QDCAsyncReader::start()
{
    emit startWork();
}

