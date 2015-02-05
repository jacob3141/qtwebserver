//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with QtWebServer.  If not, see <http://www.gnu.org/licenses/>.
//

// Own includes
#include "log.h"

// Standard includes
#include <iostream>

namespace WebServer {

WebServer::Log* WebServer::Log::_instance;

Log* Log::instance() {
    if(!_instance) {
        _instance = new Log();
    }
    return _instance;
}

void Log::log(QString name, QString message, EntryType entryType) {
    Q_UNUSED(entryType);
    switch (entryType) {
    case Information:
        std::cout << "[" << name.toStdString() << "] "
                  << message.toStdString() << std::endl;
        break;
    case Warning:
        std::cout << "[" << name.toStdString() << "] "
                  << message.toStdString() << std::endl;
        break;
    case Error:
        std::cout << "\033[1;31m[" << name.toStdString() << "] "
                  << message.toStdString() << "\033[0m" << std::endl;
        break;
    }
}

Log::Log() {
}

} // namespace WebServer
