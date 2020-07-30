#include "qdcselectionproxy.h"

QDCSelectionProxy::QDCSelectionProxy(QObject *parent)
    : QObject(parent)
{

}

void QDCSelectionProxy::start()
{
    ensureNotBusy();

    divesModel.clear();
    selected.clear();

    setBusy(true);
    emit started();
}

void QDCSelectionProxy::setDevice(DeviceData dev)
{
    setDevice(static_cast<QDeviceData>(dev));
}

void QDCSelectionProxy::setDevice(QDeviceData dev)
{
    ensureNotBusy();
    device = dev;
}

void QDCSelectionProxy::setDescriptor(QDCDescriptor *descr)
{
    ensureNotBusy();
    descriptor = descr;
}

void QDCSelectionProxy::setDescriptor(DCDeviceDescriptor *descr)
{
    setDescriptor((QDCDescriptor *)descr);
}

void QDCSelectionProxy::setBusy(bool st)
{
    if (isBusy == st) {
        return;
    }

    isBusy = st;
    emit isBusyChanged();
}

bool QDCSelectionProxy::getBusy()
{
    return isBusy;
}

bool QDCSelectionProxy::isWriteReady()
{
    return true;
}

void QDCSelectionProxy::write(DCDive *dive)
{
    write((QDCDive *)dive);
}

void QDCSelectionProxy::write(QDCDive *dive)
{
    ensureBusy();
    divesModel.add(dive);
}

void QDCSelectionProxy::end()
{
    ensureBusy();
    emit show();
}

void QDCSelectionProxy::done()
{
    setBusy(false);
    emit hide();
    emit finished();
}

void QDCSelectionProxy::cancel()
{
    divesModel.clear();
    selected.clear();

    setBusy(false);
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

QList<QDCDive *> QDCSelectionProxy::getSelected()
{

    selected.clear();
    auto data = divesModel.getData();
    for(auto dive : *data) {
        if (divesModel.isSelected(dive)) {
            selected.push_back(dive);
        }
    }
    return selected;
}

void QDCSelectionProxy::ensureNotBusy()
{
    if (isBusy) {
        throw std::runtime_error("Already started");
    }
}

void QDCSelectionProxy::ensureBusy()
{
    if (!isBusy) {
        throw std::runtime_error("Not yet started");
    }

}
