#-------------------------------------------------
#
# ALGTS Sample Script
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TEMPLATE  = app
CONFIG   += console

CONFIG(debug,   debug|release): TARGET = sampled
CONFIG(release, debug|release): TARGET = sample

SOURCES += sample.cpp

CONFIG(debug,   debug|release): LIBS += ../../../bin/libcored.a
CONFIG(release, debug|release): LIBS += ../../../bin/libcore.a
