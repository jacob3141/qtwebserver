#
# Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
#
# This file is part of WebServer.
#
# WebServer is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# WebServer is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with WebServer.  If not, see <http://www.gnu.org/licenses/>.
#

TEMPLATE = lib

QT += core network xml
QT -= gui

CONFIG   += staticlib

SOURCES += \
    statuscodes.cpp \
    networkresponse.cpp \
    networkrequest.cpp \
    logger.cpp \
    log.cpp \
    application.cpp \
    multithreadedtcpserver.cpp \
    tcpserverthread.cpp

HEADERS += \
    statuscodes.h \
    responder.h \
    networkresponse.h \
    networkrequest.h \
    logger.h \
    log.h \
    application.h \
    multithreadedtcpserver.h \
    tcpserverthread.h

RESOURCES +=
