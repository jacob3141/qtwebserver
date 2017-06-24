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

// Own includes
#include "log.h"

// Standard includes
#include <iostream>

#include <QFile>
#include <QDebug>
#include <QDateTime>

namespace QtWebServer {

QtWebServer::Log* QtWebServer::Log::_instance;

Log* Log::instance() {
    if(!_instance) {
        _instance = new Log();
    }
    return _instance;
}

Log::LoggingMode Log::loggingMode() {
    return _loggingMode.r();
}

void Log::setLoggingMode(Log::LoggingMode loggingMode) {
    _loggingMode = loggingMode;
}

void Log::setLoggingFile(QString logfile)
{
    QFile file(logfile);

    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Can`t log to file " << logfile;
        exit(EXIT_FAILURE);
    }
    _logFile = logfile;
}

void Log::log(QString name, QString message, EntryType entryType) {
    if(loggingMode() == LoggingModeNone) {
        return;
    }

    QString logMessage;

    switch (entryType) {
    case Verbose:
        logMessage = "V: ["+name+"] "+message;
        break;
    case Information:
        logMessage = "I: ["+name+"] "+message;
        break;
    case Warning:
        logMessage = "W: ["+name+"] "+message;
        break;
    case Error:
        logMessage = "E: ["+name+"] "+message;
        break;
    }

    if(loggingMode() == LoggingModeConsole)
    {
        std::cout << logMessage.toStdString() << std::endl;
    }

    if(loggingMode() == LoggingToDebug)
    {
        qDebug() << logMessage;
    }

    if(loggingMode() == LoggingToFile)
    {
        QFile logFile(_logFile);
        if(logFile.open(QIODevice::ReadWrite))
        {
            QTextStream stream(&logFile);
            stream << QDateTime::currentDateTime().toString() << " " << logMessage << endl;
            logFile.close();
        }
        else
        {
            qDebug() << "Can`t log to file " << _logFile;
            exit(EXIT_FAILURE);
        }
    }
}

Log::Log() {
    _loggingMode = LoggingModeConsole;
}

} // namespace QtWebServer
