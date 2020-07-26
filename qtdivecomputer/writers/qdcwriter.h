#ifndef QDCWRITER_H
#define QDCWRITER_H
#include <QObject>
#include "../divecomputer/writers/dcwriterinterface.h"
#include "../descriptor/qdcdescriptor.h"
#include "../device/qdcdevice.h"

class QDCWriter : public QObject, public DCWriterInterface
{
    Q_OBJECT
    Q_PROPERTY(unsigned int current READ getCurrent WRITE setCurrent NOTIFY progress)
    Q_PROPERTY(unsigned int maximum READ getMaximum WRITE setMaximum NOTIFY progress)
    Q_PROPERTY(QDCDevice *device WRITE setDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor)


public:
    QDCWriter(QObject *parent = NULL);
    void setDevice(QDCDevice *dev);
    void setDescriptor(QDCDescriptor *descr);
public slots:
    void setMaximum(unsigned int);
    unsigned int getMaximum();
    void setCurrent(unsigned int);
    unsigned int getCurrent();
signals:
    void progress(unsigned int current, unsigned int maximum);
    void finished();
private:
    unsigned int current = 0;
    unsigned int maximum = 0;
    QDCDevice *device = NULL;
    QDCDescriptor *descriptor = NULL;
};

#endif // QDCWRITER_H
