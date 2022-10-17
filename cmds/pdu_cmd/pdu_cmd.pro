QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CM_PATH = ../../cores/commons
include($$CM_PATH/rpc/json/jsonrpc.pri)

HEADERS += \
    sshrpcclient.h

SOURCES += \
        main.cpp \
        sshrpcclient.cpp

contains(TARGET_ARCH, x86_64) {
    # Default rules for deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /opt/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
} else {
    target.path = /usr/data/clever/app
    INSTALLS = target
}

