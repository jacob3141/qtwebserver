//
// Copyright 2010-2014 Jacob Dawid <jacob.dawid@cybercatalyst.net>
//
// This file is part of Shark.
//
// Shark is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shark is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shark.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SHARK_LOGGER_H
#define SHARK_LOGGER_H

// Own includes
#include "shark_log.h"

// Qt includes
#include <QString>

namespace Shark {

class Logger {
public:
    Logger(QString name);
    virtual ~Logger();

    void log(QString message, Log::EntryType entryType = Log::Information);

private:
    QString _name;
};

} // namespace Shark

#endif // SHARK_LOGGER_H
