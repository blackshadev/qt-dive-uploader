#ifndef QDCSELECTIONPROXY_H
#define QDCSELECTIONPROXY_H
#include <QObject>
#include "../divecomputer/writers/dcwriterinterface.h"
#include "../entities/qdcdive.h"
#include "../listmodels/qdcdivelistmodel.h"


class QDCSelectionProxy : public QObject, public DCWriterInterface
{
    Q_OBJECT
    Q_PROPERTY(QDCDiveListModel *model READ getModel CONSTANT)
public:
    QDCSelectionProxy(QObject *parent = NULL);
    QDCDiveListModel *getModel();
public slots:
    void start() override;
    void setDevice(DeviceData dev) override;
    void setDescriptor(DCDeviceDescriptor *descr) override;
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
    void cancelled();
    void finished(QList<QDCDive *> *);
private:
    void populateSelected();
    QList<QDCDive *> selected;
    QDCDiveListModel divesModel;
};

#endif // QDCSELECTIONPROXY_H
