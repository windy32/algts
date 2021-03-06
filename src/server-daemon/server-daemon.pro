#-------------------------------------------------
#
# ALGTS Server Daemon Project
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TEMPLATE  = app
CONFIG   += console

CONFIG(debug,   debug|release): TARGET = ../../bin/serverd-debug
CONFIG(release, debug|release): TARGET = ../../bin/serverd

SOURCES += server-daemon.cpp

CONFIG(debug,   debug|release): LIBS += ../../bin/libcored.a
CONFIG(release, debug|release): LIBS += ../../bin/libcore.a
