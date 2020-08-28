#ifndef LITTLEDIVELOGCOMPUTER_H
#define LITTLEDIVELOGCOMPUTER_H

#include <QObject>
#include <QJsonObject>
#include <QByteArray>

class LittleDiveLogComputer : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QByteArray fingerprint READ getFingerprint)
    Q_PROPERTY(int id READ getId)
    Q_PROPERTY(unsigned int serial READ getSerial)
public:
    static LittleDiveLogComputer *fromData(QJsonObject obj, QObject *parent = NULL);
    LittleDiveLogComputer(QObject *parent = NULL);
    QByteArray getFingerprint();
    int getId();
    unsigned int getSerial();

protected:
    int id;
    unsigned int serial;
    QByteArray fingerprint;

};
Q_DECLARE_METATYPE(LittleDiveLogComputer *)

#endif // LITTLEDIVELOGCOMPUTER_H
