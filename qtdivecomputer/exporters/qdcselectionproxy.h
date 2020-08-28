#ifndef QDCSELECTIONPROXY_H
#define QDCSELECTIONPROXY_H
#include <QObject>
#include "qdcwritetarget.h"
#include "../divecomputer/exporters/dcexporterinterface.h"
#include "../entities/qdcdive.h"
#include "../listmodels/qdcdivelistmodel.h"
#include "../descriptor/qdcdescriptor.h"

class QDCSelectionProxy : public QDCWriteTarget
{
    Q_OBJECT
    Q_PROPERTY(QDCDiveListModel *model READ getModel CONSTANT)
    Q_PROPERTY(QList<QDCDive *> selected READ getSelected)

public:
    QDCSelectionProxy(QObject *parent = NULL);
    QDCDiveListModel *getModel();

public slots:
    void start() override;
    void write(QDCDive *dive) override;
    void end() override;
    void cancel() override;
    void done();
    void select(QDCDive *dive);
    void deselect(QDCDive *dive);
    QList<QDCDive *> getSelected();

signals:
    void show();
    void hide();

private:
    QList<QDCDive *> selected;
    QDCDiveListModel divesModel;
    void clear();
};
Q_DECLARE_METATYPE(QList<QDCDive *>)

#endif // QDCSELECTIONPROXY_H
