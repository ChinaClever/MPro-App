INCLUDEPATH += $$PWD

#QT += mqtt
include(qmqtt/qmqtt.pri)

HEADERS += \
    $$PWD/mqtt_client.h

SOURCES += \
    $$PWD/mqtt_client.cpp
