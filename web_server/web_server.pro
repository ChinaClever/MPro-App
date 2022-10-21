QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CM_PATH = ../cores/commons
include($$CM_PATH/cfg/cfg.pri)
include(mongoose/mongoose.pri)
include($$CM_PATH/rpc/json/jsonrpc.pri)


HEADERS += \
    web_core.h \
    web_http.h \
    web_obj.h \
    web_rpc.h


SOURCES += \
        main.cpp \
        web_core.cpp \
        web_http.cpp \
        web_obj.cpp \
        web_rpc.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

