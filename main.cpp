#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QSerialPortInfo>
#include <QQuickStyle>
#include <libdivecomputer/device.h>

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
    while((status = dc_iterator_next(iterator, &descriptor)) == DC_STATUS_SUCCESS) {
        qInfo( dc_descriptor_get_product(descriptor) );
        qInfo( dc_descriptor_get_vendor(descriptor) );
    }

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("myModel", QVariant::fromValue(dataList));

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    return app.exec();
}
