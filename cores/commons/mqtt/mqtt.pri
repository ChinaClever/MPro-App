INCLUDEPATH += $$PWD

#QT += mqtt
include(qmqtt/qmqtt.pri)

HEADERS += \
    $$PWD/mqtt_client.h \
    $$PWD/mqtt_publisher.h \
    $$PWD/mqtt_subscriber.h

SOURCES += \
    $$PWD/mqtt_client.cpp \
    $$PWD/mqtt_publisher.cpp \
    $$PWD/mqtt_subscriber.cpp
