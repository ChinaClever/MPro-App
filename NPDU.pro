TEMPLATE = subdirs

unix {
SUBDIRS += \
    cores \
    cmds/pdu_cmd \
    cmds/proc_log \
    starts/start_up
}

contains(TARGET_ARCH, x86_64) {
    SUBDIRS += \
#        tools/app_pack \
        tools/ota_updater
}

