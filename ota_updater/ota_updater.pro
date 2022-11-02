QT -= gui

CONFIG += c++17 console
CONFIG -= app_bundle

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

include(common/common.pri)

HEADERS += \
    ota_core.h \
    ota_net.h \
    ota_obj.h \
    ota_usb.h \
    ota_web.h

SOURCES += \
    main.cpp \
    ota_core.cpp \
    ota_net.cpp \
    ota_obj.cpp \
    ota_usb.cpp \
    ota_web.cpp

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

