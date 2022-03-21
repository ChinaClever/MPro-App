QT += dbus

INCLUDEPATH += $$PWD

HEADERS += \
    $$PWD/dbus_call.h \
    $$PWD/dbus_object.h \
    $$PWD/dbus_receiver.h \
    $$PWD/dbus_sender.h

SOURCES += \
    $$PWD/dbus_call.cpp \
    $$PWD/dbus_object.cpp \
    $$PWD/dbus_receiver.cpp \
    $$PWD/dbus_sender.cpp
