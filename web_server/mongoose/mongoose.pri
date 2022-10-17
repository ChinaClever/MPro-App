unix {
INCLUDEPATH += $$PWD
LIBS += -lssl -lcrypto

HEADERS += \
    $$PWD/mongoose.h

SOURCES += \
    $$PWD/mongoose.c
}
