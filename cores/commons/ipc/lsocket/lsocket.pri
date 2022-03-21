QT += network
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/lsocket_client.h \
    $$PWD/lsocket_monitor.h \
    $$PWD/lsocket_object.h \
    $$PWD/lsocket_server.h

SOURCES += \
    $$PWD/lsocket_client.cpp \
    $$PWD/lsocket_monitor.cpp \
    $$PWD/lsocket_object.cpp \
    $$PWD/lsocket_server.cpp
