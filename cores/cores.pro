QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle


# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(mbs/mbs.pri)
include(cfgs/cfgs.pri)
include(logs/logs.pri)
include(apps/apps.pri)
include(rpcs/rpcs.pri)
include(odbc/odbc.pri)
include(ftps/ftps.pri)
include(aiots/aiots.pri)
include(datas/datas.pri)
include(redis/redis.pri)
include(agents/agents.pri)
include(alarms/alarms.pri)
include(cascade/cascade.pri)
include(outlets/outlets.pri)
include(commons/commons.pri)
include(integrs/integrs.pri)
include(settings/settings.pri)

SOURCES += \
        main.cpp


contains(TARGET_ARCH, x86_64) {
    # Default rules for deployment.
    qnx: target.path = /tmp/$${TARGET}/bin
    else: unix:!android: target.path = /tmp/$${TARGET}/bin
    !isEmpty(target.path): INSTALLS += target
} else {
    target.path = /tmp
    INSTALLS = target
}


