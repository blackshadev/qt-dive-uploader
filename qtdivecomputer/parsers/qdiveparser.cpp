#include "qdiveparser.h"

QDCDiveParser::QDCDiveParser(QObject *parent) : QObject(parent), DCDiveParser()
{}

DCDive *QDCDiveParser::createDive()
{
    return new QDCDive();
}
