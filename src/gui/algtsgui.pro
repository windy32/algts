#-------------------------------------------------
#
# ALGTS GUI Project
#
#-------------------------------------------------

QT       += core gui

TARGET = algtsgui
TEMPLATE = app

CONFIG(debug,   debug|release): TARGET = ../../bin/algtsgui-debug
CONFIG(release, debug|release): TARGET = ../../bin/algtsgui

SOURCES += mainwindow.cpp stacklist.cpp main.cpp
HEADERS += mainwindow.h stacklist.h

FORMS    += mainwindow.ui

CONFIG(debug,   debug|release): LIBS += ../../bin/libcored.a
CONFIG(release, debug|release): LIBS += ../../bin/libcore.a
