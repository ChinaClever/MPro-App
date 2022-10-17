unix {
INCLUDEPATH += $$PWD

#include(SMX/SMX.pri)
LIBS += -lcrypto

HEADERS += \
    $$PWD/AES/qaesencryption.h \
    $$PWD/SMX/sm4.h \
    $$PWD/sercet_tlscert.h \
    $$PWD/sercret_aes.h \
    $$PWD/sercret_rsa.h \
    $$PWD/sercret_sm.h


SOURCES += \
    $$PWD/AES/qaesencryption.cpp \
    $$PWD/SMX/sm4.cpp \
    $$PWD/sercet_tlscert.cpp \
    $$PWD/sercret_aes.cpp \
    $$PWD/sercret_rsa.cpp \
    $$PWD/sercret_sm.cpp

}

HEADERS += \
    $$PWD/sercret_core.h

SOURCES += \
    $$PWD/sercret_core.cpp

