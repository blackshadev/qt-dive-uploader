#include "qdcselectionproxy.h"

QDCSelectionProxy::QDCSelectionProxy(QObject *parent)
    : QDCWriteTarget(parent)
{}

void QDCSelectionProxy::start()
{
    ensureNotBusy();
    clear();

    setBusy();
    emit started();
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
    unsetBusy();
    emit hide();
    emit ended();
}

void QDCSelectionProxy::cancel()
{
    unsetBusy();
    clear();

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

void QDCSelectionProxy::clear()
{
    divesModel.clear();
    selected.clear();
}

