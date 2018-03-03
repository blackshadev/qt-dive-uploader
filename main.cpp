#include <string>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSortFilterProxyModel>
#include "libdivecomputer/qlibdivecomputer.h"
#include "libdivecomputer/dccomputerlist.h"
#include "sessionstore.h"
#include "littledivelog/littledivelog.h"

int main(int argc, char *argv[])
{
#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QQuickStyle::setStyle("Fusion");
    qmlRegisterType<DCComputerList>("DCComputer", 0, 1, "ComputerRoles");
    qmlRegisterType<QLibDiveComputer>("Libdivecomputer", 0, 1, "loglevel");

    QQmlApplicationEngine engine;

    SessionStore sess("./session.json");
    sess.load();

    LittleDiveLog* log = new LittleDiveLog();
    QLibDiveComputer* dc = new QLibDiveComputer();
    dc->m_path = sess.m_data.getPath();

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("session", &sess.m_data);
    ctxt->setContextProperty("libdivecomputer", dc);
    ctxt->setContextProperty("littledivelog", log);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int res = app.exec();

    sess.save();

    delete dc;
    return res;
}
