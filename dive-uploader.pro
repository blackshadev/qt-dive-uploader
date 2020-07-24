include(vendor/vendor.pri)

ICON = $$PWD/resources/icon.ico
QMAKE_TARGET_COMPANY = "Littledev"
QMAKE_TARGET_DESCRIPTION = "Uploader tool for the littledevs DiveLogbook"
RC_ICONS = $$ICON
unix:OS = "unix"
win32:OS = "win32"
macx:OS = "macx"

QT += qml quick serialport quickcontrols2 network
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += main.cpp \
    divecomputer/reader/dcreader.cpp \
    divecomputer/utils/dcstatus.cpp \
    divecomputer/context/dccontext.cpp \
    divecomputer/descriptor/dcdevicedescriptor.cpp \
    divecomputer/devices/dcdevice.cpp \
    divecomputer/devices/dcusbhiddevice.cpp \
    divecomputer/entities/dcdive.cpp \
    divecomputer/entities/dcdivesample.cpp \
    divecomputer/entities/dcdivesampleevent.cpp \
    divecomputer/parsers/dcdiveparser.cpp \
    divecomputer/parsers/dcdivesampleparser.cpp \
    divecomputer/transports/dctransport.cpp \
    divecomputer/transports/dcusbhidtransport.cpp \
    divecomputer/transports/transporttype.cpp \
    qtdivecomputer/context/qdccontext.cpp \
    qtdivecomputer/descriptor/qdcdescriptor.cpp \
    qtdivecomputer/device/qdcdevice.cpp \
    qtdivecomputer/entities/qdcdive.cpp \
    qtdivecomputer/listmodels/qdcdescriptorlistmodel.cpp \
    qtdivecomputer/listmodels/qdcdevicelistmodel.cpp \
    qtdivecomputer/listmodels/qdcdivelistmodel.cpp \
    qtdivecomputer/listmodels/qdcloglevellistmodel.cpp \
    qtdivecomputer/listmodels/qdctransportlistmodel.cpp \
    qtdivecomputer/parsers/qdiveparser.cpp \
    qtdivecomputer/reader/qdcasyncreader.cpp \
    qtdivecomputer/reader/qdcreader.cpp \
    qtdivecomputer/transports/qdctransport.cpp \
    serializer/dcdivesampleeventserializer.cpp \
    serializer/dcdivesampleserializer.cpp \
    serializer/dcdiveserializer.cpp \
    serializer/optionalserializer.cpp \
    sessionstore.cpp \
    littledivelog/littledivelog.cpp \
    jsonrequest.cpp \
    littledivelog/userinfo.cpp \
    common/datetime.cpp

HEADERS += \
    divecomputer/common.h \
    divecomputer/context/dccontextinterface.h \
    divecomputer/devices/dcdeviceinterface.h \
    divecomputer/parsers/dcdiveparserinterface.h \
    divecomputer/reader/dcreader.h \
    divecomputer/reader/dcreaderinterface.h \
    divecomputer/utils//dcstatus.h \
    common/vector.h \
    divecomputer/context/dccontext.h \
    divecomputer/descriptor/dcdevicedescriptor.h \
    divecomputer/descriptor/devicedescriptorinterface.h \
    divecomputer/devices/dcdevice.h \
    divecomputer/devices/dcusbhiddevice.h \
    divecomputer/entities/dcdive.h \
    divecomputer/entities/dcdivesample.h \
    divecomputer/entities/dcdivesampleevent.h \
    divecomputer/parsers/dcdiveparser.h \
    divecomputer/parsers/dcdivesampleparser.h \
    divecomputer/transports/dctransport.h \
    divecomputer/transports/dctransportinterface.h \
    divecomputer/transports/dcusbhidtransport.h \
    divecomputer/transports/transporttype.h \
    qtdivecomputer/context/qdccontext.h \
    qtdivecomputer/descriptor/qdcdescriptor.h \
    qtdivecomputer/device/qdcdevice.h \
    qtdivecomputer/entities/qdcdive.h \
    qtdivecomputer/listmodels/qdcdescriptorlistmodel.h \
    qtdivecomputer/listmodels/qdcdevicelistmodel.h \
    qtdivecomputer/listmodels/qdcdivelistmodel.h \
    qtdivecomputer/listmodels/qdcloglevellistmodel.h \
    qtdivecomputer/listmodels/qdctransportlistmodel.h \
    qtdivecomputer/parsers/qdiveparser.h \
    qtdivecomputer/reader/qdcasyncreader.h \
    qtdivecomputer/reader/qdcreader.h \
    qtdivecomputer/transports/qdctransport.h \
    serializer/dcdivesampleeventserializer.h \
    serializer/dcdivesampleserializer.h \
    serializer/dcdiveserializer.h \
    serializer/optionalserializer.h \
    sessionstore.h \
    common/list.h \
    common/optional.h \
    common/list.h \
    common/list.hpp \
    littledivelog/littledivelog.h \
    jsonrequest.h \
    littledivelog/userinfo.h \
    common/datetime.h


RESOURCES += qml.qrc

DESTDIR = bin
OBJECTS_DIR = .obj
MOC_DIR = .moc
RCC_DIR = .rcc
UI_DIR = .ui

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

INCLUDEPATH += include
DEPENDPATH += include


unix|macx {
    LIBS += -L"$$PWD/lib/linux/" -ldivecomputer
    PRE_TARGETDEPS += $$PWD/lib/linux/libdivecomputer.a

    copylibs.commands += mkdir -p $$shell_path($$DESTDIR/lib) $$escape_expand(\\n\\t)
    copylibs.commands += $(COPY_DIR) $$shell_path($$PWD/lib/linux/libdivecomputer.so*) $$shell_path($$DESTDIR/lib) $$escape_expand(\\n\\t)
    first.depends = $(first) copylibs
    export(first.depends)
    QMAKE_EXTRA_TARGETS += first copylibs
}

win32 {
    OPENSSL_PATH = C:/Program Files (x86)/OpenSSL-Win32
    LIBS += -L"$$OPENSSL_PATH/lib/MinGW" -lssl -lcrypto
    TARGET_EXT = .exe
    LIBS += -L"$$PWD/lib/win32/" -ldivecomputer
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer-0.dll
    PRE_TARGETDEPS += $$PWD/lib/win32/libusb-1.0.dll
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer.lib
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer.def
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer.dll
    copylibs.commands += $(COPY_DIR) $$shell_path($$PWD/lib/win32/libdivecomputer-0.dll) $$shell_path($$DESTDIR) $$escape_expand(\\n\\t)
    copylibs.commands += $(COPY_DIR) $$shell_path($$PWD/lib/win32/libusb-1.0.dll) $$shell_path($$DESTDIR) $$escape_expand(\\n\\t)
    copylibs.commands += $(COPY_DIR) $$shell_quote($$shell_path($$OPENSSL_PATH/bin/libcrypto-1_1.dll)) $$shell_path($$DESTDIR) $$escape_expand(\\n\\t)
    copylibs.commands += $(COPY_DIR) $$shell_quote($$shell_path($$OPENSSL_PATH/bin/libssl-1_1.dll)) $$shell_path($$DESTDIR) $$escape_expand(\\n\\t)

    first.depends = $(first) copylibs
    export(first.depends)
    QMAKE_EXTRA_TARGETS += first copylibs
}

macx:DEPLOY_COMMAND = macdeployqt
win32:DEPLOY_COMMAND = windeployqt

copy_extra_files.commands = $(COPY_DIR) $$shell_path($$PWD/resources) $$shell_path($$DESTDIR)
QMAKE_EXTRA_TARGETS += copy_extra_files

DEPLOY_TARGET = $$OUT_PWD/$$DESTDIR/$${TARGET}$${TARGET_EXT}
win32|macx {
    deploy.commands = $${DEPLOY_COMMAND} --qmldir $$PWD  $$shell_quote($$shell_path($${DEPLOY_TARGET}))
}
unix {
    deploy.commands = $$PWD/tools/linux-deploy.sh $$DEPLOY_TARGET
}

deploy.depends = first copy_extra_files
QMAKE_EXTRA_TARGETS += deploy


PACKAGE_CONFIG = $$PWD/packaging/config/config.xml
PACKAGE_DIR = $$PWD/packaging/packages
PACKAGE_DATA_DIR = $$PACKAGE_DIR/nl.littledev.diveloguploader/data
PACKAGE_DATA_TARGET = $$PACKAGE_DATA_DIR/$${TARGET}.7z
win32:INSTALLER_TARGET = ".exe"
INSTALLER_TARGET = $$PWD/packaging/$$TARGET-$${OS}-installer$${INSTALLER_EXT}
unix:QTIFW = ~/Qt/Tools/QtInstallerFramework/3.2/bin/

ARCHIVEGEN = $${QTIFW}archivegen
BINARYCREATOR = $${QTIFW}binarycreator

package.depends = deploy

package.commands += $(DEL_FILE) $$shell_path($$OUT_PWD/$$DESTDIR/session.json) $$escape_expand(\\n\\t)
package.commands += $(DEL_FILE) $$shell_quote($$shell_path($${PACKAGE_DATA_TARGET})) $$escape_expand(\\n\\t)
package.commands += $${ARCHIVEGEN} $$shell_quote($$shell_path($${PACKAGE_DATA_TARGET})) $$shell_path($$OUT_PWD/$$DESTDIR)/* $$escape_expand(\\n\\t)

QMAKE_EXTRA_TARGETS += package

installer.commands = $${BINARYCREATOR} -c $$shell_quote($$shell_path($${PACKAGE_CONFIG})) -p $$shell_quote($$shell_path($$PACKAGE_DIR)) $$shell_quote($$shell_path($$INSTALLER_TARGET))
installer.depends = package
QMAKE_EXTRA_TARGETS += installer


DISTFILES += \
    tools/linux-deploy.sh \
    tools/linux-run.sh



