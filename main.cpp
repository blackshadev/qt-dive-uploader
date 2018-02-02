#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QSerialPortInfo>
#include <QQuickStyle>
#include <libdivecomputer/device.h>
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

    dc_descriptor_t* descriptor;
    dc_iterator_t* iterator;
    dc_status_t status;
    dc_descriptor_iterator(&iterator);
    QStringList compList;

    QLibDiveComputer::update_version();

    while((status = dc_iterator_next(iterator, &descriptor)) == DC_STATUS_SUCCESS) {
        QString s = QString();
        s += dc_descriptor_get_vendor(descriptor);
        s += ' ';
        s += dc_descriptor_get_product(descriptor);
        compList.append(s);
    }
    compList.sort();

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("dc_available_computers", QVariant::fromValue(compList));
    ctxt->setContextProperty("dc_version", QVariant::fromValue(QLibDiveComputer::version));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
