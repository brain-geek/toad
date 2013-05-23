#-------------------------------------------------
#
# Project created by QtCreator 2013-04-20T12:38:23
#
#-------------------------------------------------

QT       += core
QT       += webkit

TARGET = toad
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += main.cpp \
    loadtestingbrowser.cpp \
    toadapplication.cpp \
    logger.cpp

HEADERS += \
    loadtestingbrowser.h \
    toadapplication.h \
    logger.h

OTHER_FILES += \
    startme.sh \
    README.md
