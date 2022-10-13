TEMPLATE = subdirs

unix {
SUBDIRS += \
    cores \
    start_up \
    cmds/pdu_cmd \
    cmds/proc_log \
    cmds/proc_run \
    ota_updater/ota_net \
    ota_updater/ota_usb
}




