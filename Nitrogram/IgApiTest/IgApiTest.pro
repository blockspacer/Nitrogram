#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T00:36:56
#
#-------------------------------------------------

QT       += testlib

QT       -= gui

TARGET = IgApiTest
CONFIG   += console testcase
CONFIG   -= app_bundle

TEMPLATE = app

INCLUDEPATH += $$OUT_PWD/../../_out/include

LIBS += -L$$OUT_PWD/../../_out/lib \
        -lIgApi

SOURCES += IgApiTest.cpp

PRE_TARGETDEPS += $$OUT_PWD/../../_out/lib/libIgApi.a

target.path = $$OUT_PWD/../../_out/test

INSTALLS += target
