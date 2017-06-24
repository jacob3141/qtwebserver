//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with QtWebServer.
// If not, see <http://www.gnu.org/licenses/>.
//
// It is possible to obtain a commercial license of QtWebServer.
// Please contact Jacob Dawid <jacob@omg-it.works>
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
        LoggingModeConsole,
        LoggingToDebug,
        LoggingToFile
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
    void setLoggingFile(QString logfile);

protected:
    void log(QString name, QString message, EntryType entryType);

private:
    Log();

    ThreadGuard<LoggingMode> _loggingMode;

    static Log* _instance;
    QString _logFile;
};

} // namespace QtWebServer
