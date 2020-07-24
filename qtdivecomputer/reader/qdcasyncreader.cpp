#include "qdcasyncreader.h"

QDCAsyncReader::QDCAsyncReader()
{
    qInfo() << "Construct " << QThread::currentThreadId();
    workerThread = new QThread;
    reader = new QDCReader;
    reader->moveToThread(workerThread);
    reader->connect(this, SIGNAL(startWork()), reader, SLOT(startReading()));
    reader->connect(reader, SIGNAL(progress(unsigned int, unsigned int)), this, SIGNAL(progress(unsigned int, unsigned int)));
    reader->connect(reader, SIGNAL(clock(unsigned int, dc_ticks_t)), this, SIGNAL(clock(unsigned int, dc_ticks_t)));
    reader->connect(reader, SIGNAL(deviceInfo(unsigned int, unsigned int, unsigned int)), this, SIGNAL(deviceInfo(unsigned int, unsigned int, unsigned int)));
    reader->connect(reader, SIGNAL(waiting()), this, SIGNAL(waiting()));
    reader->connect(reader, SIGNAL(dive(QDCDive *)), this, SIGNAL(dive(QDCDive *)));
    reader->connect(reader, SIGNAL(error(QString)), this, SIGNAL(error(QString)));
    reader->connect(reader, SIGNAL(finished()), this, SIGNAL(finished()));
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

DCReaderInterface *QDCAsyncReader::setContext(DCContextInterface *c)
{
    context = c;
    reader->setContext(context);
    return this;
}

void QDCAsyncReader::startReading()
{
    emit startWork();
}

void QDCAsyncReader::cancel()
{
    reader->cancel();
}

