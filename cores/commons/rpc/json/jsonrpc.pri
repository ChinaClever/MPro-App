INCLUDEPATH += $$PWD

include($$PWD/jcon/jcon.pri)

HEADERS += \
    $$PWD/jsonrpc_client.h \
    $$PWD/jsonrpc_server.h

SOURCES += \
    $$PWD/jsonrpc_client.cpp \
    $$PWD/jsonrpc_server.cpp
