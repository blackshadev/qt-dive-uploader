#ifndef QDCSELECTIONPROXY_H
#define QDCSELECTIONPROXY_H
#include <QObject>
#include "../divecomputer/writers/dcwriterinterface.h"
#include "../entities/qdcdive.h"
#include "../listmodels/qdcdivelistmodel.h"
#include "../descriptor/qdcdescriptor.h"

class QDCSelectionProxy : public QObject, public DCWriterInterface
{
    Q_OBJECT
    Q_PROPERTY(QDCDiveListModel *model READ getModel CONSTANT)
    Q_PROPERTY(QDeviceData device WRITE setDevice)
    Q_PROPERTY(QDCDescriptor *descriptor WRITE setDescriptor)
    Q_PROPERTY(bool isBusy READ getBusy NOTIFY isBusyChanged)

public:
    QDCSelectionProxy(QObject *parent = NULL);

    QDCDiveListModel *getModel();
    void setDevice(DeviceData dev) override;
    void setDevice(QDeviceData device);
    void setDescriptor(QDCDescriptor *descr);
    void setDescriptor(DCDeviceDescriptor *descr) override;
    void setBusy(bool st);
    bool getBusy();

public slots:
    void start() override;
    void write(DCDive *dive) override;
    void write(QDCDive *dive);
    void end() override;
    void done();
    void cancel();
    void select(QDCDive *dive);
    void deselect(QDCDive *dive);
signals:
    void show();
    void hide();
    void started();
    void cancelled();
    void finished(QList<QDCDive *> *selected);
    void isBusyChanged();
private:
    void populateSelected();
    QList<QDCDive *> selected;
    QDCDiveListModel divesModel;
    QDCDescriptor *descriptor;
    QDeviceData device;
    bool isBusy = false;
    void ensureNotBusy();
    void ensureBusy();
};

#endif // QDCSELECTIONPROXY_H
