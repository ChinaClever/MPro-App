TEMPLATE = subdirs

unix {
SUBDIRS += \
    cores \
    start_up \
    web_server \
    cmds/pdu_cmd \
    cmds/proc_run \
    ota_updater/ota_net \
#    ota_updater/ota_usb
}






