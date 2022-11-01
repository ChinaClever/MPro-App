
# for some reason with Travis' qt 5.0.2 you can't chain these with an |
#NEED_GCOV_SUPPORT = 0
#greaterThan(QT_MAJOR_VERSION, 4):lessThan(QT_MINOR_VERSION, 2) {
#    NEED_GCOV_SUPPORT = 1
#}
#lessThan(QT_MAJOR_VERSION, 5):lessThan(QT_MINOR_VERSION, 9):lessThan(QT_PATCH_VERSION, 6) {
#    NEED_GCOV_SUPPORT = 1
#}

#greaterThan(NEED_GCOV_SUPPORT, 0) {
#    # NOTE: remove when travis adds a newer ubuntu, or when hell freezes over
#    gcov {
#        QMAKE_CFLAGS           += -fprofile-arcs -ftest-coverage
#        QMAKE_CXXFLAGS         += -fprofile-arcs -ftest-coverage
#        QMAKE_OBJECTIVE_CFLAGS += -fprofile-arcs -ftest-coverage
#        QMAKE_LFLAGS           += -fprofile-arcs -ftest-coverage
#        QMAKE_CLEAN += $(OBJECTS_DIR)*.gcno and $(OBJECTS_DIR)*.gcda
#    }
#}

#Define GIT Macros
#GIT_VERSION = $$system(git describe --long --dirty --tags)
#DEFINES += GIT_VERSION=\\\"$$GIT_VERSION\\\"

#GIT_TAG = $$system(git describe --abbrev=0)
#VERSION = $$replace(GIT_TAG, v,)

HEADERS +=   \
    $$PWD/qamqpauthenticator.h \
    $$PWD/qamqpchannel.h \
    $$PWD/qamqpchannel_p.h \
    $$PWD/qamqpchannelhash_p.h \
    $$PWD/qamqpclient.h \
    $$PWD/qamqpclient_p.h \
    $$PWD/qamqpexchange.h \
    $$PWD/qamqpexchange_p.h \
    $$PWD/qamqpframe_p.h \
    $$PWD/qamqpglobal.h \
    $$PWD/qamqpmessage.h \
    $$PWD/qamqpmessage_p.h \
    $$PWD/qamqpqueue.h \
    $$PWD/qamqpqueue_p.h \
    $$PWD/qamqptable.h

SOURCES +=  \
    $$PWD/qamqpauthenticator.cpp \
    $$PWD/qamqpchannel.cpp \
    $$PWD/qamqpchannelhash.cpp \
    $$PWD/qamqpclient.cpp \
    $$PWD/qamqpexchange.cpp \
    $$PWD/qamqpframe.cpp \
    $$PWD/qamqpmessage.cpp \
    $$PWD/qamqpqueue.cpp \
    $$PWD/qamqptable.cpp

