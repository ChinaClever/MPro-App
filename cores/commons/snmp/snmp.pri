unix{
include(agent/agent.pri)
include(client/client.pri)

#INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/snmpagent.h \
    $$PWD/snmpclient.h

SOURCES += \
    $$PWD/snmpagent.cpp \
    $$PWD/snmpclient.cpp
}
