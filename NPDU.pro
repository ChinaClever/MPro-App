TEMPLATE = subdirs

unix {
SUBDIRS += \
    cores \
    ssh/pdu_cmd \
    http_server \
    starts/start_up
}

SUBDIRS += \
    tools/ota_updater
