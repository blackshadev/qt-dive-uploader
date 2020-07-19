#include "qdiveparser.h"

QDiveParser::QDiveParser(QObject *parent) : QObject(parent), DiveParser()
{}

DCDive *QDiveParser::createDive()
{
    return new QDCDive(parent());
}
