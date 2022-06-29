#INCLUDEPATH += $$PWD
#qtHaveModule(websockets): QMQTT_WEBSOCKETS: QT += websockets
DEFINES += QT_NO_CAST_TO_ASCII QT_NO_CAST_FROM_ASCII
DEFINES += MQTT_PROJECT_INCLUDE_SRC QT_WEBSOCKETS_LIB
# https://github.com/emqx/qmqtt

PUBLIC_HEADERS += \
    $$PWD/qmqtt_client.h \
    $$PWD/qmqtt_frame.h \
    $$PWD/qmqtt_message.h \
    $$PWD/qmqtt_routesubscription.h \
    $$PWD/qmqtt_routedmessage.h \
    $$PWD/qmqtt_router.h \
    $$PWD/qmqtt_networkinterface.h \
    $$PWD/qmqtt_socketinterface.h \
    $$PWD/qmqtt_timerinterface.h

PRIVATE_HEADERS += \
    $$PWD/qmqtt_client_p.h \
    $$PWD/qmqtt_message_p.h \
    $$PWD/qmqtt_network_p.h \
    $$PWD/qmqtt_socket_p.h \
    $$PWD/qmqtt_timer_p.h

SOURCES += \
    $$PWD/qmqtt_client_p.cpp \
    $$PWD/qmqtt_client.cpp \
    $$PWD/qmqtt_frame.cpp \
    $$PWD/qmqtt_message.cpp \
    $$PWD/qmqtt_network.cpp \
    $$PWD/qmqtt_routesubscription.cpp \
    $$PWD/qmqtt_router.cpp \
    $$PWD/qmqtt_socket.cpp \
    $$PWD/qmqtt_timer.cpp

#QMQTT_WEBSOCKETS {
    PRIVATE_HEADERS += \
        $$PWD/qmqtt_websocket_p.h \
        $$PWD/qmqtt_websocketiodevice_p.h

    SOURCES += \
        $$PWD/qmqtt_websocket.cpp \
        $$PWD/qmqtt_websocketiodevice.cpp
#}

!contains(CONFIG, QMQTT_NO_SSL) {
    PRIVATE_HEADERS += \
        $$PWD/qmqtt_ssl_socket_p.h

    SOURCES += \
        $$PWD/qmqtt_ssl_socket.cpp
}

HEADERS += \
    $$PWD/qmqtt_global.h \
    $$PWD/qmqtt.h

HEADERS += $$PUBLIC_HEADERS $$PRIVATE_HEADERS
