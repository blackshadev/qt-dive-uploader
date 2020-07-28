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
    write((QDCDive *)dive);
}

void QDCSelectionProxy::write(QDCDive *dive)
{
    divesModel.add(dive);
}

void QDCSelectionProxy::end()
{
    emit show();
}

void QDCSelectionProxy::done()
{
    emit hide();
    populateSelected();
    emit finished(&selected);
}

void QDCSelectionProxy::cancel()
{
    emit hide();
    emit cancelled();
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

void QDCSelectionProxy::populateSelected()
{
    selected.clear();
    auto data = divesModel.getData();
    for(auto dive : *data) {
        selected.push_back(dive);
    }
}
