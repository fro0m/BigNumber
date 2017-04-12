TEMPLATE = app
CONFIG += c++14 exceptions_off rtti_off qt
CONFIG += thread

SOURCES += main.cpp \

# All includes. It is put in separate module as it is used with unit tests
include($$top_srcdir/includes/includes.pri)

