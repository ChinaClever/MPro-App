INCLUDEPATH += $$PWD

include($$PWD/qamqp/qamqp.pri )

HEADERS +=   \
    $$PWD/qrabbitmq.h

SOURCES +=   \
    $$PWD/qrabbitmq.cpp
