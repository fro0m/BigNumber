DEFINES += UNIT_TESTING_MODE

include($$top_srcdir/includes/includes.pri)

CONFIG -= app_bundle
CONFIG += console c++14
CONFIG += qt

QT += testlib

TEMPLATE = app
SOURCES += \
    testbignumber.cpp

