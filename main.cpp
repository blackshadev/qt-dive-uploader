#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QSerialPortInfo>
#include <QQuickStyle>
#include <string>
#include "qlibdivecomputer.h"

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



    QLibDiveComputer* dc = new QLibDiveComputer();
    QStringList* compList = dc->get_devices();

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("dc_available_computers", QVariant::fromValue(*compList));
    ctxt->setContextProperty("dc_version", QVariant::fromValue(dc->version));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int res = app.exec();

    delete compList;
    delete dc;

    return res;
}
