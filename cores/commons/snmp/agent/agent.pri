#INCLUDEPATH += $$PWD

LIBS += -lnetsnmp -lnetsnmpagent

HEADERS += \
    $$PWD/QSNMP.h \
    $$PWD/snmpmodule.h

SOURCES += \
    $$PWD/QSNMP.cpp \
    $$PWD/snmpmodule.cpp
