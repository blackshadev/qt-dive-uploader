#ifndef QDCDIVE_H
#define QDCDIVE_H
#include <QObject>
#include "../divecomputer/entities/dcdive.h"

class QDCDive : public QObject, public DCDive
{
    Q_OBJECT
public:
    QDCDive(QObject *parent = NULL);
};
Q_DECLARE_METATYPE(QDCDive *)

#endif // QDCDIVE_H
