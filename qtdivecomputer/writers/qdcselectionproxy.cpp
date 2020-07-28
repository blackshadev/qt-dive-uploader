#include "qdcselectionproxy.h"

QDCSelectionProxy::QDCSelectionProxy(QObject *parent)
    : QObject(parent)
{

}

void QDCSelectionProxy::start()
{}

void QDCSelectionProxy::setDevice(DeviceData dev)
{}

void QDCSelectionProxy::setDescriptor(DCDeviceDescriptor *descr)
{}

void QDCSelectionProxy::write(DCDive *dive)
{

}

void QDCSelectionProxy::end()
{

}

QDCDiveListModel *QDCSelectionProxy::getModel()
{
    return &divesModel;
}

void QDCSelectionProxy::select(QDCDive *dive)
{
    divesModel.select(dive);
}

void QDCSelectionProxy::deselect(QDCDive *dive)
{
    divesModel.deselect(dive);
}
