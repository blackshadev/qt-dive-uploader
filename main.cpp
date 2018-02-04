#include <string>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QSerialPortInfo>
#include <QQuickStyle>
#include <QSortFilterProxyModel>
#include "qlibdivecomputer.h"
#include "sessionstore.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QQuickStyle::setStyle("Fusion");

    QQmlApplicationEngine engine;

    QStringList dataList;
    QList<QSerialPortInfo> data = QSerialPortInfo::availablePorts();
    for(QSerialPortInfo info : data ) {
       dataList.append(info.portName());
    }

    SessionStore sess("./session.json");
    sess.load();

    QLibDiveComputer* dc = new QLibDiveComputer();
    DCComputerList* compList = dc->get_devices();

    QSortFilterProxyModel sortedCompList;
    sortedCompList.setSourceModel(compList);
    sortedCompList.setSortRole(DCComputerList::ComputerRoles::DescriptionRole);
    sortedCompList.setDynamicSortFilter(true);
    sortedCompList.sort(0);


    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("session", &sess.data);
    ctxt->setContextProperty("libdivecomputer", dc);
    ctxt->setContextProperty("dc_available_computers", &sortedCompList);
    ctxt->setContextProperty("dc_version", QVariant::fromValue(dc->version));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int res = app.exec();

    sess.save();

    delete compList;
    delete dc;

    return res;
}
