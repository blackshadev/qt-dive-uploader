#include "qdiveparser.h"

QDCDiveParser::QDCDiveParser(QObject *parent) : QObject(parent), DCDiveParser()
{}

QDCDiveParser *QDCDiveParser::setDevice(QDCDevice *device)
{
    DCDiveParser::setDevice(device);
    return this;
}

DCDive *QDCDiveParser::createDive()
{
    return new QDCDive();
}
