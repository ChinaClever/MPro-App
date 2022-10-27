
QT += sql

include(dbs/dbs.pri)


INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/log_core.h \
    $$PWD/log_read.h \
    $$PWD/log_sys.h

SOURCES += \
    $$PWD/log_core.cpp \
    $$PWD/log_read.cpp \
    $$PWD/log_sys.cpp
