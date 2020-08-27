#ifndef DIRECTORIES_H
#define DIRECTORIES_H

#include <QObject>

class Directories : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString userHome READ getUserHome CONSTANT)
public:
    Directories();
public slots:
    QString getUserHome();
    QString join(QStringList list);
};

#endif // DIRECTORIES_H
