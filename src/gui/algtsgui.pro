#-------------------------------------------------
#
# ALGTS GUI Project
#
#-------------------------------------------------

QT       += core gui network sql

TARGET = algtsgui
TEMPLATE = app

CONFIG(debug,   debug|release): TARGET = ../../bin/algtsgui-debug
CONFIG(release, debug|release): TARGET = ../../bin/algtsgui

SOURCES += mainwindow.cpp main.cpp \
    scenarioex.cpp \
    widget/scenarioview.cpp \
    test.cpp
SOURCES += linegraph.cpp globaldatabase.cpp script.cpp

SOURCES +=
SOURCES += widget/stacklist.cpp
SOURCES += widget/ipaddresswidget.cpp
SOURCES += widget/cdfwidget.cpp
SOURCES += widget/pdfwidget.cpp
SOURCES += widget/unaryfunctionwidget.cpp
SOURCES += widget/binaryfunctionwidget.cpp

SOURCES += dialog/ipaddrdialog.cpp
SOURCES += dialog/emulatordialog.cpp
SOURCES += dialog/distributiondialog.cpp
SOURCES += dialog/selectscenariodialog.cpp
SOURCES += dialog/selectscriptdialog.cpp
SOURCES += dialog/savetestdialog.cpp
SOURCES += dialog/ratingmethoddialog.cpp

HEADERS += mainwindow.h \
    scenarioex.h \
    widget/scenarioview.h \
    test.h
HEADERS += linegraph.h globaldatabase.h script.h

HEADERS +=
HEADERS += widget/stacklist.h
HEADERS += widget/ipaddresswidget.h
HEADERS += widget/cdfwidget.h
HEADERS += widget/pdfwidget.h
HEADERS += widget/unaryfunctionwidget.h
HEADERS += widget/binaryfunctionwidget.h

HEADERS += dialog/ipaddrdialog.h
HEADERS += dialog/emulatordialog.h
HEADERS += dialog/distributiondialog.h
HEADERS += dialog/selectscenariodialog.h
HEADERS += dialog/selectscriptdialog.h
HEADERS += dialog/savetestdialog.h
HEADERS += dialog/ratingmethoddialog.h

FORMS += mainwindow.ui

FORMS += dialog/ipaddrdialog.ui
FORMS += dialog/emulatordialog.ui
FORMS += dialog/distributiondialog.ui
FORMS += dialog/selectscenariodialog.ui
FORMS += dialog/selectscriptdialog.ui
FORMS += dialog/savetestdialog.ui
FORMS += dialog/ratingmethoddialog.ui

CONFIG(debug,   debug|release): LIBS += ../../bin/libcored.a
CONFIG(release, debug|release): LIBS += ../../bin/libcore.a

