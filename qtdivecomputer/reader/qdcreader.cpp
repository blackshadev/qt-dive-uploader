#include "qdcreader.h"

QDCReader::QDCReader(QObject *parent) : QObject(parent), QThread(parent), DCReader()
{

}

QDCReader *QDCReader::setDevice(QDCDevice *d)
{
    DCReader::setDevice((DCDevice *)d);
    return this;
}

QDCReader *QDCReader::setParser(QDiveParser *p)
{
    DCReader::setParser((DiveParser *)p);
    return this;
}

void QDCReader::run()
{
    DCReader::start();
}
