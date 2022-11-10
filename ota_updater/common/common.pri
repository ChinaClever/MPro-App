QT += network
INCLUDEPATH += $$PWD

CM_PATH = ../../cores/commons
include($$CM_PATH/cfg/cfg.pri)
include($$CM_PATH/cms/cms.pri)
include($$CM_PATH/ipc/ipc.pri)
#include($$CM_PATH/crc/crc.pri)
include($$CM_PATH/dtls/dtls.pri)
include($$CM_PATH/ssdp/ssdp.pri)
include($$CM_PATH/cthread/cthread.pri)
include($$CM_PATH/network/network.pri)

HEADERS += \
    $$PWD/otassdp.h

SOURCES += \
    $$PWD/otassdp.cpp
