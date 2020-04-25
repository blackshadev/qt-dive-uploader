
ICON = $$PWD/resources/icon.ico
QMAKE_TARGET_COMPANY = "Littledev"
QMAKE_TARGET_DESCRIPTION = "Uploader tool for the littledevs DiveLogbook"
RC_ICONS = $$ICON

QT += qml quick serialport quickcontrols2 network
CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += main.cpp \
    libdivecomputer/qlibdivecomputer.cpp \
    libdivecomputer/dccomputerlist.cpp \
    sessionstore.cpp \
    libdivecomputer/dcdownloadcontext.cpp \
    libdivecomputer/dive.cpp \
    libdivecomputer/writer/divewriter.cpp \
    libdivecomputer/writer/filewriter.cpp \
    littledivelog/littledivelog.cpp \
    jsonrequest.cpp \
    littledivelog/userinfo.cpp \
    libdivecomputer/writer/littlelogwriter.cpp \
    libdivecomputer/writer/jsondivewriter.cpp \
    common/base64.cpp \
    libdivecomputer/qdivemodel.cpp \
    common/datetime.cpp

HEADERS += \
    libdivecomputer/qlibdivecomputer.h \
    libdivecomputer/dccomputerlist.h \
    sessionstore.h \
    libdivecomputer/dcdownloadcontext.h \
    libdivecomputer/dive.h \
    common/list.h \
    common/optional.h \
    common/list.h \
    common/list.hpp \
    libdivecomputer/writer/divewriter.h \
    libdivecomputer/writer/filewriter.h \
    littledivelog/littledivelog.h \
    jsonrequest.h \
    littledivelog/userinfo.h \
    libdivecomputer/writer/littlelogwriter.h \
    libdivecomputer/writer/jsondivewriter.h \
    common/base64.h \
    libdivecomputer/qdivemodel.h \
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

include(vendor/vendor.pri)

INCLUDEPATH += include
DEPENDPATH += include

unix {
    LIBS += -L"$$PWD/lib/linux/" -ldivecomputer
    PRE_TARGETDEPS += $$PWD/lib/linux/libdivecomputer.a
}

win32 {
    TARGET_EXT = .exe
    LIBS += -L"$$PWD/lib/win32/" -ldivecomputer
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer-0.dll
    PRE_TARGETDEPS += $$PWD/lib/win32/libusb-1.0.dll
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer.lib
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer.def
    PRE_TARGETDEPS += $$PWD/lib/win32/libdivecomputer.dll
    copylibs.commands += $(COPY_DIR) $$shell_path($$PWD/lib/win32/libdivecomputer-0.dll) $$shell_path($$DESTDIR) $$escape_expand(\\n\\t)
    copylibs.commands += $(COPY_DIR) $$shell_path($$PWD/lib/win32/libusb-1.0.dll) $$shell_path($$DESTDIR) $$escape_expand(\\n\\t)

    first.depends = $(first) copylibs
    export(first.depends)
    export(copylibs.commands)
    QMAKE_EXTRA_TARGETS += first copylibs

}

macx:DEPLOY_COMMAND = macdeployqt
win32:DEPLOY_COMMAND = windeployqt
unix:DEPLOY_COMMAND = linuxdeployqt

copy_extra_files.commands = $(COPY_DIR) $$shell_path($$PWD/resources) $$shell_path($$DESTDIR)
QMAKE_EXTRA_TARGETS += copy_extra_files

DEPLOY_TARGET = $$OUT_PWD/$$DESTDIR/$${TARGET}$${TARGET_EXT}
deploy.commands = $${DEPLOY_COMMAND} --qmldir $$PWD  $$shell_quote($$shell_path($${DEPLOY_TARGET}))
deploy.depends = $(first) copy_extra_files
QMAKE_EXTRA_TARGETS += deploy


PACKAGE_CONFIG = $$PWD/packaging/config/config.xml
PACKAGE_DIR = $$PWD/packaging/packages
PACKAGE_DATA_DIR = $$PACKAGE_DIR/nl.littledev.diveloguploader/data
PACKAGE_DATA_TARGET = $$PACKAGE_DATA_DIR/$${TARGET}.7z
INSTALLER_TARGET = $$PWD/packaging/diveuploader-installer.exe

package.commands = archivegen $$shell_quote($$shell_path($${PACKAGE_DATA_TARGET})) $$shell_path($$OUT_PWD/$$DESTDIR)/*
#package.depends = deploy
QMAKE_EXTRA_TARGETS += package

installer.commands = binarycreator -c $$shell_quote($$shell_path($${PACKAGE_CONFIG})) -p $$shell_quote($$shell_path($$PACKAGE_DIR)) $$shell_quote($$shell_path($$INSTALLER_TARGET))
#installer.depends = package
QMAKE_EXTRA_TARGETS += installer




DISTFILES += \
    qmldir



