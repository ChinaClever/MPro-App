unix {
INCLUDEPATH += $$PWD

include(SMX/SMX.pri)


HEADERS += \
    $$PWD/AES/qaesencryption.h \
    $$PWD/SMX/simplesm4.h \
    $$PWD/sercet_tlscert.h \
    $$PWD/sercret_aes.h \
    $$PWD/qtsecret_global.h \
    $$PWD/sercret_rsa.h \
    $$PWD/sercret_sm.h


SOURCES += \
    $$PWD/AES/qaesencryption.cpp \
    $$PWD/SMX/simplesm4.cpp \
    $$PWD/sercet_tlscert.cpp \
    $$PWD/sercret_aes.cpp \
    $$PWD/sercret_rsa.cpp \
    $$PWD/sercret_sm.cpp

}

HEADERS += \
    $$PWD/sercret_core.h

SOURCES += \
    $$PWD/sercret_core.cpp

