
DEFINES += QPM_INIT\\(E\\)=\"E.addImportPath(QStringLiteral(\\\"qrc:/\\\"));\"
DEFINES += QPM_USE_NS
INCLUDEPATH += $$PWD
QML_IMPORT_PATH += $$PWD


include($$PWD/font/awesome/pri/fontawesome.pri)
include($$PWD/fr/grecko/sortfilterproxymodel/SortFilterProxyModel.pri)
