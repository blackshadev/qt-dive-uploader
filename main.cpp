#include <string>
#include <QWindow>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSortFilterProxyModel>
#include "sessionstore.h"
#include "littledivelog/littledivelog.h"
#include "jsonrequest.h"
#include "qtdivecomputer/context/qdccontext.h"
#include "qtdivecomputer/listmodels/qdcdevicelistmodel.h"
#include "qtdivecomputer/listmodels/qdcdescriptorlistmodel.h"
#include "qtdivecomputer/listmodels/qdctransportlistmodel.h"
#include "qtdivecomputer/listmodels/qdcloglevellistmodel.h"
#include "qtdivecomputer/listmodels/qdcdivelistmodel.h"
#include "qtdivecomputer/parsers/qdiveparser.h"

int main(int argc, char *argv[])
{

    LittleDiveLog log;

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
    qmlRegisterType<QDCDescriptorListModel>("Libdivecomputer", 0, 2, "DescriptorRoles");
    qmlRegisterType<QDCTransportListModel>("Libdivecomputer", 0, 2, "TransportRoles");
    qmlRegisterType<QDCDeviceListModel>("Libdivecomputer", 0, 2, "DeviceRoles");
    qmlRegisterType<QDCLogLevelListModel>("Libdivecomputer", 0, 2, "LogLevelRoles");
    qmlRegisterType<QDCDeviceListModel>("Libdivecomputer", 0, 2, "QDCDeviceListModel");
    qmlRegisterType<QDCLogLevelListModel>("Libdivecomputer", 0, 2, "QDCLogLevelListModel");
    qmlRegisterType<QDCContext>("Libdivecomputer", 0, 2, "QDCContext");
    qmlRegisterType<QDCTransportListModel>("Libdivecomputer", 0, 2, "QDCTransportListModel");
    qmlRegisterType<QDCDescriptorListModel>("Libdivecomputer", 0, 2, "QDCDescriptorListModel");
    qmlRegisterType<QDCDiveListModel>("Libdivecomputer", 0, 2, "QDCDiveListModel");

    QQmlApplicationEngine engine;

    QPM_INIT(engine)

    log.set_refresh_token(sess.m_data.getRefreshToken());

    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("session", &sess.m_data);
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
