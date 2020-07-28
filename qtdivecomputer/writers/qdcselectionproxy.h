#ifndef QDCSELECTIONPROXY_H
#define QDCSELECTIONPROXY_H
#include <QObject>
#include "../divecomputer/writers/dcwriterinterface.h"
#include "../entities/qdcdive.h"
#include "../listmodels/qdcdivelistmodel.h"


class QDCSelectionProxy : public QObject, public DCWriterInterface
{
    Q_OBJECT
    Q_PROPERTY(QDCDiveListModel * model READ model CONSTANT)
public:
    QDCSelectionProxy(QObject *parent = NULL);
    void start() override;
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void write(DCDive *dive) override;
    void end() override;
public slots:
    void select(QDCDive *dive);
    void deselect(QDCDive *dive);
private:
    QDCDiveListModel divesModel;
};

#endif // QDCSELECTIONPROXY_H
