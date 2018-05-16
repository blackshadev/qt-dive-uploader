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
    qmlRegisterType<WriteType>("Libdivecomputer", 0, 1, "WriteTypes");

    QQmlApplicationEngine engine;

    SessionStore sess("./session.json");
    sess.load();

    LittleDiveLog* log = new LittleDiveLog();
    QLibDiveComputer* dc = new QLibDiveComputer();
    dc->bind_littledivelog(log);

    dc->set_writeTypeAsString(sess.m_data.getWriteType(), WriteType::File);

    dc->connect(dc, &QLibDiveComputer::writeTypeChanged, [&sess, dc](WriteType::writetype t) {
        sess.m_data.setWriteType(dc->get_writeTypeAsString());
    });

    dc->m_path = sess.m_data.getPath();
    log->set_refresh_token(sess.m_data.getRefreshToken());

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
