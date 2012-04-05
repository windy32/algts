#-------------------------------------------------
#
# ALGTS Core Project
#
#-------------------------------------------------

QT       += core network
QT       -= gui

TEMPLATE  = lib
CONFIG   += staticlib

CONFIG(debug,   debug|release): TARGET = ../../../bin/cored
CONFIG(release, debug|release): TARGET = ../../../bin/core

# c++ constructs used by all modules
SOURCES += global-timer.cpp log.cpp QBoundTcpSocket.cpp scenario.cpp trace.cpp
HEADERS += global-timer.h   log.h   QBoundTcpSocket.h   scenario.h   trace.h

SOURCES += rng-stream.cpp random-variable.cpp random-variable-factory.cpp
HEADERS += rng-stream.h   random-variable.h   random-variable-factory.h

# tasks
SOURCES += task/task.cpp task/bulk-download-task.cpp task/bulk-upload-task.cpp
HEADERS += task/task.h   task/bulk-download-task.h   task/bulk-upload-task.h

SOURCES += task/onoff-download-task.cpp task/tcp-echo-task.cpp
HEADERS += task/onoff-download-task.h   task/tcp-echo-task.h

SOURCES += task/async-udp-echo-task.cpp task/null-task.cpp
HEADERS += task/async-udp-echo-task.h   task/null-task.h

# servers
SOURCES += server/server.cpp server/tcp-server.cpp
HEADERS += server/server.h   server/tcp-server.h

SOURCES += server/bulk-download-server.cpp server/bulk-upload-server.cpp
HEADERS += server/bulk-download-server.h   server/bulk-upload-server.h

SOURCES += server/onoff-download-server.cpp server/tcp-echo-server.cpp
HEADERS += server/onoff-download-server.h   server/tcp-echo-server.h

SOURCES += server/async-udp-echo-server.cpp server/server-factory.cpp
HEADERS += server/async-udp-echo-server.h   server/server-factory.h

# clients
SOURCES += client/client.cpp
HEADERS += client/client.h

SOURCES += client/bulk-download-client.cpp client/bulk-upload-client.cpp
HEADERS += client/bulk-download-client.h   client/bulk-upload-client.h

SOURCES += client/onoff-download-client.cpp client/tcp-echo-client.cpp
HEADERS += client/onoff-download-client.h   client/tcp-echo-client.h

SOURCES += client/async-udp-echo-client.cpp client/client-factory.cpp
HEADERS += client/async-udp-echo-client.h   client/client-factory.h

# emulators
SOURCES += emulator/emulator.cpp
HEADERS += emulator/emulator.h

SOURCES += emulator/netem-emulator.cpp emulator/nistnet-emulator.cpp
HEADERS += emulator/netem-emulator.h   emulator/nistnet-emulator.h

# terminals
SOURCES += terminal/terminal.cpp
HEADERS += terminal/terminal.h

SOURCES += terminal/telnet-terminal.cpp terminal/ssh-terminal.cpp
HEADERS += terminal/ssh-terminal.h      terminal/ssh-terminal.h

# applications
SOURCES += app/core-application.cpp app/console-application.cpp
HEADERS += app/core-application.h   app/console-application.h
