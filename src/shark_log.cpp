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

// Own includes
#include "shark_log.h"

// Standard includes
#include <iostream>

namespace Shark {

Shark::Log* Shark::Log::_instance;

Log* Log::instance() {
    if(!_instance) {
        _instance = new Log();
    }
    return _instance;
}

void Log::log(QString name, QString message, EntryType entryType) {
    Q_UNUSED(entryType);
    std::cout << "[" << name.toStdString() << "] "
              << message.toStdString() << std::endl;
}

Log::Log() {
}

} // namespace Shark
