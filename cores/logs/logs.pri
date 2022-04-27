
QT += sql

include(dbs/dbs.pri)


INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/log_core.h

SOURCES += \
    $$PWD/log_core.cpp
