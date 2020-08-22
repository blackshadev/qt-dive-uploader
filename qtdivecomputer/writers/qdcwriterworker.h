#ifndef QDCQUEUEDWRITER_H
#define QDCQUEUEDWRITER_H
#include <QQueue>
#include <QObject>
#include "../entities/qdcdive.h"
#include "../writers/qdcwriter.h"

class QDCWriterWorker : public QObject
{
    Q_OBJECT
public:
    QDCWriterWorker(QObject *parent = NULL);
    void setDevice(DeviceData data);
    void setDescriptor(QDCDescriptor *descriptor);
    void setWriter(QDCWriter *writer);

signals:
    void dive(DCDive *);
    void cancel();
    void start();
    void end();

    void written();
    void ended();
    void started();
    void cancelled();
    void error(QString);

protected slots:
    void receiveCancel();
    void receiveStart();
    void receiveEnd();
    void receiveDive(DCDive* dive);
    void consume();

protected:
    void performEnd();

private:
    QQueue<DCDive *> queue;
    QDCWriter *writer = NULL;
    DeviceData deviceData;
    QDCDescriptor *descriptor;
    bool isEnded = false;
    bool isStarted = false;
};

#endif // QDCQUEUEDWRITER_H
