
include(db/db.pri)
include(sql/sql.pri)
include(cfg/cfg.pri)
include(ipc/ipc.pri)
include(http/http.pri)
include(ssdp/ssdp.pri)
include(snmp/snmp.pri)
include(dtls/dtls.pri)
include(down/down.pri)
include(smtp/smtp.pri)
include(mqtt/mqtt.pri)
include(secret/secret.pri)
include(serial/serial.pri)
include(modbus/modbus.pri)
include(cthread/cthread.pri)
include(network/network.pri)
include(mongoose/mongoose.pri)
include(websocket/websocket.pri)

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/commons.h \
    $$PWD/datapacket.h

SOURCES += \
    $$PWD/commons.cpp
