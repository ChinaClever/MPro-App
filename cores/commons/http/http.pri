INCLUDEPATH += $$PWD

include($$PWD/JQLibrary/JQLibrary.pri )

HEADERS +=  \
    $$PWD/httpclient.h \
    $$PWD/httpserver.h

SOURCES +=  \
    $$PWD/httpclient.cpp \
    $$PWD/httpserver.cpp
