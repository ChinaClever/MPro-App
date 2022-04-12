QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(core/core.pri)
include(home/home.pri)
include(common/common.pri)
include(remote/remote.pri)
include(progres/progres.pri)

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    navbarwid.cpp

HEADERS += \
    mainwindow.h \
    navbarwid.h

FORMS += \
    mainwindow.ui \
    navbarwid.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    images/image.qrc