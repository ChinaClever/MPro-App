INCLUDEPATH += $$PWD

include($$PWD/JQLibrary/JQLibrary.pri )

HEADERS +=  \
    $$PWD/http.h \
    $$PWD/httpclient.h \
    $$PWD/httpserver.h

SOURCES +=  \
    $$PWD/http.cpp \
    $$PWD/httpserver.cpp
