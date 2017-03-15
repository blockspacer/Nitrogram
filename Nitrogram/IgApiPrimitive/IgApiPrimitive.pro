QT += core network
QT -= gui

QMAKE_CXXFLAGS_WARN_OFF -= -Wunused-parameter

CONFIG += c++11

TARGET = IgApiPrimitive
CONFIG += console
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    RequestManager.cpp \
    RequestReply.cpp \
    IgApiUtil.cpp \
    RequestCookieJar.cpp \
    JsonWalker.cpp \
    IgApi.cpp \
    HumanEmulator.cpp \
    IgObjects.cpp

HEADERS += pt.h \
    RequestManager.h \
    IgConst.h \
    RequestReply.h \
    IgApiUtil.h \
    RequestCookieJar.h \
    JsonWalker.h \
    IgApi.h \
    HumanEmulator.h \
    IgObjects.h

INCLUDEPATH += $$OUT_PWD/../../_out/include

LIBS += -L$$OUT_PWD/../../_out/lib \
        -lIgApi

PRE_TARGETDEPS += $$OUT_PWD/../../_out/lib/libIgApi.a

target.path = $$OUT_PWD/../../_out/bin

INSTALLS += target
