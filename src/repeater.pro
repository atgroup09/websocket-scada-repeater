#-------------------------------------------------
#
# Project created by QtCreator 2019-04-18T09:00:00
#
#-------------------------------------------------

QT += core websockets
QT -= gui

TARGET  = repeater
CONFIG += console
CONFIG += c++11
CONFIG -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    log.cpp \
    args.cpp \
    config.cpp \
    client.cpp \
##  service.cpp \
    server.cpp

HEADERS += \
    log.h \
    args.h \
    global.h \
    config.h \
    client.h \
##  service.h \
    server.h


# QtService
# include files
## INCLUDEPATH += lib/qtservice/src
# shared libraries for platforms
CONFIG(debug, debug|release) {
# debug
    win32: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-headd
##  unix: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-head
} else {
# release
    win32: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-head
##  unix: LIBS += -L$$PWD/lib/qtservice -lQtSolutions_Service-head
}
