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
    littledivelog/littledivelog.cpp

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
    littledivelog/littledivelog.h


RESOURCES += qml.qrc
DESTDIR = bin

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

include(vendor/vendor.pri)

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/release/ -ldivecomputer
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../usr/local/lib/debug/ -ldivecomputer
else:unix: LIBS += -L$$PWD/../../../../usr/local/lib/ -ldivecomputer

INCLUDEPATH += $$PWD/../../../../usr/local/include
DEPENDPATH += $$PWD/../../../../usr/local/include

win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/libdivecomputer.a
else:win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/libdivecomputer.a
else:win32:!win32-g++:CONFIG(release, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/release/divecomputer.lib
else:win32:!win32-g++:CONFIG(debug, debug|release): PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/debug/divecomputer.lib
else:unix: PRE_TARGETDEPS += $$PWD/../../../../usr/local/lib/libdivecomputer.a

DISTFILES += \
    qmldir
