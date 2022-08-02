unix {
INCLUDEPATH += $$PWD


HEADERS += \
    $$PWD/AES/qaesencryption.h \
    $$PWD/RSA/qrsaencryption.h \
    $$PWD/SMX/base64.h \
    $$PWD/SMX/sm2.h \
    $$PWD/SMX/sm3.h \
    $$PWD/SMX/sm3_hash.h \
    $$PWD/SMX/sm4.h \
    $$PWD/SMX/stringutil.h \
    $$PWD/gmp/bigint.h \
    $$PWD/gmp/mini-gmp.h \
    $$PWD/gmp/minigmp_global.h \
    $$PWD/sercet_tlscert.h \
    $$PWD/sercret_aes.h \
    $$PWD/qtsecret_global.h \
    $$PWD/sercret_rsa.h \
    $$PWD/sercret_smx.h

SOURCES += \
    $$PWD/AES/qaesencryption.cpp \
    $$PWD/RSA/qrsaencryption.cpp \
    $$PWD/SMX/base64.cpp \
    $$PWD/SMX/sm2.cpp \
    $$PWD/SMX/sm3.cpp \
    $$PWD/SMX/sm3_hash.cpp \
    $$PWD/SMX/sm4.cpp \
    $$PWD/SMX/stringutil.cpp \
    $$PWD/gmp/bigint.cpp \
    $$PWD/gmp/mini-gmp.c \
    $$PWD/sercet_tlscert.cpp \
    $$PWD/sercret_aes.cpp \
#    $$PWD/sercret_example.cpp \
    $$PWD/sercret_rsa.cpp \
    $$PWD/sercret_smx.cpp

}
