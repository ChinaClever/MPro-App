QT *= network

#INCLUDEPATH += $$PWD
#DEPENDPATH += $$PWD
#DEFINES += HTTP

HEADERS += \
    $$PWD/cachedhttp.h \
    $$PWD/http.h \
    $$PWD/httpreply.h \
    $$PWD/httprequest.h \
    $$PWD/localcache.h \
    $$PWD/networkhttpreply.h \
    $$PWD/throttledhttp.h

SOURCES += \
    $$PWD/cachedhttp.cpp \
    $$PWD/http.cpp \
    $$PWD/httpreply.cpp \
    $$PWD/localcache.cpp \
    $$PWD/networkhttpreply.cpp \
    $$PWD/throttledhttp.cpp
