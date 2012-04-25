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

SOURCES += mainwindow.cpp stacklist.cpp main.cpp \
    ipaddrdialog.cpp \
    emulatordialog.cpp \
    distributiondialog.cpp \
    selectscenariodialog.cpp \
    selectscriptdialog.cpp \
    savetestdialog.cpp \
    ratingmethoddialog.cpp
HEADERS += mainwindow.h stacklist.h \
    ipaddrdialog.h \
    emulatordialog.h \
    distributiondialog.h \
    selectscenariodialog.h \
    selectscriptdialog.h \
    savetestdialog.h \
    ratingmethoddialog.h

FORMS    += mainwindow.ui \
    ipaddrdialog.ui \
    emulatordialog.ui \
    distributiondialog.ui \
    selectscenariodialog.ui \
    selectscriptdialog.ui \
    savetestdialog.ui \
    ratingmethoddialog.ui

CONFIG(debug,   debug|release): LIBS += ../../bin/libcored.a
CONFIG(release, debug|release): LIBS += ../../bin/libcore.a
