QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(common/common.pri)



SOURCES += \
    envwid.cpp \
    keythread.cpp \
    linewid.cpp \
    loopwid.cpp \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    envwid.h \
    keythread.h \
    linewid.h \
    loopwid.h \
    mainwindow.h

FORMS += \
    envwid.ui \
    linewid.ui \
    loopwid.ui \
    mainwindow.ui

# Default rules for deployment.
contains(TARGET_ARCH, x86_64) {
    # Default rules for deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /tmp
    !isEmpty(target.path): INSTALLS += target
} else {
    target.path = /tmp
    INSTALLS = target
}
