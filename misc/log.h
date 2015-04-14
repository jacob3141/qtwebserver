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

#pragma once

// Own includes
#include "misc/threadsafety.h"

// Qt includes
#include <QString>

namespace QtWebServer {

class Logger;
class Log {
    friend class Logger;
public:
    enum LoggingMode {
        LoggingModeNone,
        LoggingModeConsole
    };

    enum EntryType {
        Verbose     = 0,
        Information = 1,
        Warning     = 2,
        Error       = 3
    };

    static Log* instance();

    LoggingMode loggingMode();
    void setLoggingMode(LoggingMode loggingMode);

protected:
    void log(QString name, QString message, EntryType entryType);

private:
    Log();

    ThreadGuard<LoggingMode> _loggingMode;

    static Log* _instance;
};

} // namespace QtWebServer
