#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T00:07:45
#
#-------------------------------------------------

QT       -= gui

TARGET = IgApi

TEMPLATE = lib
CONFIG += staticlib

SOURCES += igapi.cpp

HEADERS += igapi.h

exportHeaders.path    = $$OUT_PWD/../../_out/include
exportHeaders.files   += $$HEADERS
target.path = $$OUT_PWD/../../_out/lib

INSTALLS += target exportHeaders
