//
// Copyright 2010-2013 Jacob Dawid <jacob.dawid@cybercatalyst.net>
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

#ifndef HTTPREQUEST_H
#define HTTPREQUEST_H

// Qt includes
#include <QString>

namespace Shark {

namespace Http {

/**
 * @brief The Request class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class Request {
public:
    Request();
    Request(QString request);
};

} // namspace Http

} // namespace Shark

#endif // HTTPREQUEST_H
