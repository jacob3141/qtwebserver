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

#ifndef SHARK_HTTPREQUEST_H
#define SHARK_HTTPREQUEST_H

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
    enum Method {
        Unknown,
        Options,
        Get,
        Head,
        Post,
        Put,
        Delete,
        Trace,
        Connect
    };

    Request(QString requestString);

    /**
     * This method is used to indicate whether a request could not be
     * interpreted correctly. You are supposed to check this first before
     * using any other methods.
     * @returns true, if the request has been valid, false otherwise.
     */
    bool validRequest();

    /**
     * @returns the method for this request.
     */
    Method method();

    /**
     * @returns the URI for this request.
     */
    QString uniqueResourceIdentifier();

    /**
     * @returns the HTTP version for this request.
     */
    QString httpVersion();

    /**
     * @returns the original request string.
     */
    QString requestString();

private:
    QString _requestString;
    Method _method;
    QString _uniqueResourceIdentifier;
    QString _httpVersion;
    bool _validRequest;
};

} // namspace Http

} // namespace Shark

#endif // SHARK_HTTPREQUEST_H
