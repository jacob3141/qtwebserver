#
# Copyright 2010-2013 Jacob Dawid <jacob.dawid@cybercatalyst.net>
#
# This file is part of Shark.
#
# Shark is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# Shark is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with Shark.  If not, see <http://www.gnu.org/licenses/>.
#

QT += core network

QT -= gui

TARGET = ../bin/shark
CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app


SOURCES += \
    service.cpp \
    webservicethread.cpp \
    main.cpp \
    httprequest.cpp \
    httpresponse.cpp

HEADERS += \
    service.h \
    webservicethread.h \
    httprequest.h \
    httpresponse.h \
    httpstatuscodes.h
