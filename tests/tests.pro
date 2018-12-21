QT       += testlib

QT       -= gui

TARGET = tst_manoeuvretest
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += tst_manoeuvretest.cpp
INCLUDEPATH += ../app

DEFINES += SRCDIR=\\\"$$PWD/\\\"
