 QT += websockets

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/ws_client.h \
    $$PWD/ws_object.h \
    $$PWD/ws_server.h

SOURCES += \
    $$PWD/ws_client.cpp \
    $$PWD/ws_object.cpp \
    $$PWD/ws_server.cpp

RESOURCES += \
    $$PWD/securesocketclient.qrc

