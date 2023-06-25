#INCLUDEPATH += $$PWD

include(LinkSDK/LinkSDK.pri)

HEADERS += \
    $$PWD/aiot_core.h \
    $$PWD/aiot_devcfg.h \
    $$PWD/aiot_line.h \
    $$PWD/aiot_loop.h \
    $$PWD/aiot_object.h \
    $$PWD/aiot_outlet.h \
    $$PWD/aiot_total.h


SOURCES += \
    $$PWD/aiot_core.cpp \
    $$PWD/aiot_devcfg.cpp \
    $$PWD/aiot_line.cpp \
    $$PWD/aiot_loop.cpp \
    $$PWD/aiot_object.cpp \
    $$PWD/aiot_outlet.cpp \
    $$PWD/aiot_total.cpp

