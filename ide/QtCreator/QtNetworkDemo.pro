QT += network
QT += core

TARGET = QtNetworkDemo
CONFIG += sdk_no_version_check

TEMPLATE = app

INCLUDEPATH += ../../src/global

HEADERS += \
    ../../src/server/QTNServerThread.h \
    ../../src/server/QTNServer.h \
    ../../src/global/QTNConst.h \
    ../../src/client/QTNClient.h \
    ../../src/QTNetworkDemo.h

SOURCES += \
    ../../src/server/QTNServerThread.cpp \
    ../../src/server/QTNServer.cpp \
    ../../src/client/QTNClient.cpp \
    ../../src/global/QTNConst.cpp \
    ../../src/QTNetworkDemo.cpp
