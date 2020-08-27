#include "directories.h"
#include <QStandardPaths>
#include <QDir>

Directories::Directories() : QObject()
{

}

QString Directories::getUserHome()
{
    return QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
}

QString Directories::join(QStringList list)
{
    if (list.size() < 1) {
        return "";
    }

    QString str = list[0];
    for (int iX = 1; iX < list.size(); iX++) {
        str += QDir::separator() + list[iX];
    }

    return str;
}
