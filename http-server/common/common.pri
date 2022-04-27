INCLUDEPATH += $$PWD

include(mongoose/mongoose.pri)

CM_PATH = ../../cores/commons
include($$CM_PATH/cms/cms.pri)
include($$CM_PATH/crc/crc.pri)
include($$CM_PATH/ipc/ipc.pri)
include($$CM_PATH/cthread/cthread.pri)
include($$CM_PATH/../ipcs/clients/clients.pri)

HEADERS +=

SOURCES +=
