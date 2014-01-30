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
#include "shark_httprequest.h"

// Qt includes
#include <QStringList>

namespace Shark {

namespace Http {

Request::Request(QString requestString) {
    _validRequest = true;
    _method = Unknown;
    _uniqueResourceIdentifier = "";
    _httpVersion = "";

    QStringList lines = requestString.split("\r\n");
    if(lines.size() < 1) {
        // If we have zero request lines, something went horribly wrong.
        // Nevertheless, it's better to stop it here.
        _validRequest = false;
        return;
    }

    QStringList requestLine = lines.at(0).split(QRegExp("\\s+"));

    if(requestLine.size() < 3) {
        // The request line has to contain three strings: The method
        // string, the request uri and the HTTP version. If we were
        // strict, we shouldn't even accept anything larger than four
        // strings, but we're permissive here.
        _validRequest = false;
        return;
    }

    QString methodString = requestLine.at(0).toLower();

    if(methodString == "options") {
        _method = Options;
    } else if(methodString == "get") {
        _method = Get;
    } else if(methodString == "head") {
        _method = Head;
    } else if(methodString == "post") {
        _method = Post;
    } else if(methodString == "put") {
        _method = Put;
    } else if(methodString == "delete") {
        _method = Delete;
    } else if(methodString == "trace") {
        _method = Trace;
    } else if(methodString == "connect") {
        _method = Connect;
    } else {
        _method = Unknown;
    }

    _uniqueResourceIdentifier = requestLine.at(1);
    _httpVersion = requestLine.at(2);
}

bool Request::validRequest() {
    return _validRequest;
}

Request::Method Request::method() {
    return _method;
}

QString Request::uniqueResourceIdentifier() {
    return _uniqueResourceIdentifier;
}

QString Request::httpVersion() {
    return _httpVersion;
}

} // namespace Http

} // namespace Shark
