#
# Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
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


TEMPLATE = lib

QT += core network xml

QT -= gui

CONFIG   += console staticlib
CONFIG   -= app_bundle

SOURCES += \
    NetworkResponse.cpp \
    Application.cpp \
    StatusCodes.cpp \
    Log.cpp \
    Logger.cpp \
    NetworkService.cpp \
    NetworkServiceThread.cpp \
    NetworkRequest.cpp

HEADERS += \
    Application.h \
    Responder.h \
    Log.h \
    Logger.h \
    NetworkService.h \
    NetworkResponse.h \
    NetworkRequest.h \
    StatusCodes.h \
    NetworkServiceThread.h

RESOURCES +=
