#include <string>
#include <QWindow>
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
#include "libdivecomputer/qdivemodel.h"
#include "jsonrequest.h"


int main(int argc, char *argv[])
{

    LittleDiveLog log;
    QLibDiveComputer dc;

    QString homeFolder = QStandardPaths::writableLocation(QStandardPaths::HomeLocation);
    QString sessionFilePath = QDir(QDir::cleanPath(homeFolder + QDir::separator() + "LittleDiveLog")).filePath("session.json");
    SessionStore sess(sessionFilePath);
    sess.load();

#if defined(Q_OS_WIN)
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling);
#endif

    QGuiApplication app(argc, argv);
    app.setOrganizationName("Littledev");
    app.setOrganizationDomain("littledev.nl");

    QQuickWindow::setTextRenderType(QQuickWindow::NativeTextRendering);
    QQuickStyle::setStyle("Material");
    qmlRegisterType<DCComputerList>("DCComputer", 0, 1, "ComputerRoles");
    qmlRegisterType<QLibDiveComputer>("Libdivecomputer", 0, 1, "LogLevel");
    qmlRegisterType<WriteType>("Libdivecomputer", 0, 1, "WriteTypes");
    qmlRegisterType<QDiveModel>("Libdivecomputer", 0, 1, "DiveModel");

    QQmlApplicationEngine engine;

    QPM_INIT(engine)

    dc.bind_littledivelog(&log);

    dc.set_writeTypeAsString(sess.m_data.getWriteType(), WriteType::File);

    dc.connect(&dc, &QLibDiveComputer::writeTypeChanged, [&sess, &dc](WriteType::writetype t) {
        sess.m_data.setWriteType(dc.get_writeTypeAsString());
    });

    dc.m_path = sess.m_data.getPath();
    log.set_refresh_token(sess.m_data.getRefreshToken());

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("session", &sess.m_data);
    ctxt->setContextProperty("libdivecomputer", &dc);
    ctxt->setContextProperty("littledivelog", &log);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    app.connect(&app, &QGuiApplication::lastWindowClosed, &app, [=]() {
        JsonRequest::stop();
    });

    int res = app.exec();

    sess.save();

    return res;
}
