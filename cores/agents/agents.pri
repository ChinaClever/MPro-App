unix {
INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/agent_get.h \
    $$PWD/agent_obj.h

SOURCES += \
    $$PWD/agent_get.cpp \
    $$PWD/agent_obj.cpp
}

HEADERS += \
    $$PWD/agent_core.h \
    $$PWD/agent_mib.h \
    $$PWD/agent_set.h \
    $$PWD/agent_trap.h

SOURCES += \
    $$PWD/agent_core.cpp \
    $$PWD/agent_mib.cpp \
    $$PWD/agent_set.cpp \
    $$PWD/agent_trap.cpp
