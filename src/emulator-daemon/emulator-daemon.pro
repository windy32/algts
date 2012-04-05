#-------------------------------------------------
#
# ALGTS Emulator Daemon Project
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TEMPLATE  = app
CONFIG   += console

CONFIG(debug,   debug|release): TARGET = ../../../bin/emulator-daemon-debug
CONFIG(release, debug|release): TARGET = ../../../bin/emulator-daemon

SOURCES += emulator-daemon-session.cpp netem-session.cpp emulator-daemon.cpp 
HEADERS += emulator-daemon-session.h   netem-session.h

CONFIG(debug,   debug|release): LIBS += ../../bin/libcored.a
CONFIG(release, debug|release): LIBS += ../../bin/libcore.a
