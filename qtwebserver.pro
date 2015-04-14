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

QT += core network xml sql
QT -= gui

CONFIG += staticlib

SOURCES += \
    http/httprequest.cpp \
    http/httpstatuscodes.cpp \
    http/httpwebengine.cpp \
    tcp/tcpmultithreadedserver.cpp \
    tcp/tcpserverthread.cpp \
    misc/log.cpp \
    misc/logger.cpp \
    http/httpresource.cpp \
    http/httpiodeviceresource.cpp \
    sql/sqlconnectionpool.cpp \
    html/htmldocument.cpp \
    util/utilassetsresource.cpp \
    http/httpresponse.cpp

HEADERS += \
    http/httprequest.h \
    http/httpresponder.h \
    http/httpstatuscodes.h \
    http/httpwebengine.h \
    tcp/tcpserverthread.h \
    tcp/tcpmultithreadedserver.h \
    tcp/tcpresponder.h \
    misc/threadsafety.h \
    misc/logger.h \
    misc/log.h \
    http/httpresource.h \
    http/httpiodeviceresource.h \
    sql/sqlconnectionpool.h \
    html/htmldocument.h \
    util/utilassetsresource.h \
    http/httpresponse.h

RESOURCES +=
