#-------------------------------------------------
#
# Project created by QtCreator 2013-11-19T20:52:46
#
#-------------------------------------------------

QT       += core network

QT -= gui

TARGET = shark
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    service.cpp \
    webservicethread.cpp \
    main.cpp

HEADERS += \
    service.h \
    webservicethread.h
