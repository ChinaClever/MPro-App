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
include(ipcs/ipcs.pri)
include(rpcs/rpcs.pri)
include(datas/datas.pri)
include(agents/agents.pri)
include(alarms/alarms.pri)
include(cascade/cascade.pri)
include(outputs/outputs.pri)
include(commons/commons.pri)
include(integrs/integrs.pri)
include(settings/settings.pri)

SOURCES += \
        main.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#target.path = /root
#INSTALLS += target
