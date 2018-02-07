#include <string>
#include <QGuiApplication>
#include <QQuickWindow>
#include <QQmlApplicationEngine>
#include <QQuickView>
#include <QQmlContext>
#include <QQuickStyle>
#include <QSortFilterProxyModel>
#include "libdivecomputer/qlibdivecomputer.h"
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

    SessionStore sess("./session.json");
    sess.load();

    QLibDiveComputer* dc = new QLibDiveComputer();

//    QSortFilterProxyModel sortedCompList;
//    sortedCompList.setSourceModel(compList);
//    sortedCompList.setSortRole(DCComputerList::ComputerRoles::DescriptionRole);
//    sortedCompList.setDynamicSortFilter(true);
//    sortedCompList.sort(0);



    QQmlContext *ctxt = engine.rootContext();
    ctxt->setContextProperty("session", &sess.m_data);
    ctxt->setContextProperty("libdivecomputer", dc);

    engine.load(QUrl(QStringLiteral("qrc:/main.qml")));
    if (engine.rootObjects().isEmpty())
        return -1;

    int res = app.exec();

    sess.save();

    delete dc;
    return res;
}
