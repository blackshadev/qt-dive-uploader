#ifndef QDCDIVEPARSER_H
#define QDCDIVEPARSER_H
#include <QObject>
#include "../divecomputer/parsers/dcdiveparser.h"
#include "../entities/qdcdive.h"

class QDiveParser : public QObject, public DiveParser
{
    Q_OBJECT
public:
    QDiveParser(QObject *parent = NULL);
protected:
    DCDive *createDive() override;
};
Q_DECLARE_METATYPE(QDiveParser *)

#endif // QDCDIVEPARSER_H
