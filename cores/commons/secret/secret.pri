unix {
INCLUDEPATH += $$PWD
#CONFIG += crypto
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
    $$PWD/RSA/bigint.h \
    $$PWD/RSA/mini-gmp.h \
    $$PWD/RSA/qrsaencryption.h \
    $$PWD/pbkdf/pbkdf2_sha256.h \
    $$PWD/pbkdf/sha256.h \
    $$PWD/sercret_core.h \
    $$PWD/sercret_pwd.h

SOURCES += \
    $$PWD/RSA/bigint.cpp \
    $$PWD/RSA/mini-gmp.c \
    $$PWD/RSA/qrsaencryption.cpp \
    $$PWD/pbkdf/sha256.cpp \
    $$PWD/sercret_core.cpp \
    $$PWD/sercret_pwd.cpp

