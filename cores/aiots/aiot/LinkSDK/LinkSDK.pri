
#CONFIG += console
include(pd/pd.pri)
INCLUDEPATH += $$PWD/include
LIBS += -L$$PWD/lib -laiot -lnopoll

HEADERS += \
    $$PWD/aiot_linksdk.h


SOURCES += \
    $$PWD/aiot_linksdk.cpp

