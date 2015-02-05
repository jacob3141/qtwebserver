//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
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
#include "networkrequest.h"

// Qt includes
#include <QStringList>

namespace Shark {

NetworkRequest::NetworkRequest(QString requestString)
    : Logger("Shark::Http::Request") {
    _requestString = requestString;
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

    QStringList splittedURI = requestLine.at(1).split('?', QString::SkipEmptyParts);
    _queryString = "";
    if(splittedURI.size() > 1) {
        _queryString = splittedURI.at(1);
        QStringList queryParameterAssignmentList = _queryString.split('&', QString::SkipEmptyParts);
        foreach(QString queryParameterAssignment, queryParameterAssignmentList) {
            QStringList assignment = queryParameterAssignment.split('=', QString::SkipEmptyParts);
            if(assignment.size() > 0) {
                _queryParameters[assignment.at(0)] = assignment.at(1);
            }
        }
    }

    _uniqueResourceIdentifier = splittedURI.at(0);

    _httpVersion = requestLine.at(2);
}

bool NetworkRequest::validRequest() {
    return _validRequest;
}

NetworkRequest::Method NetworkRequest::method() {
    return _method;
}

QString NetworkRequest::uniqueResourceIdentifier() {
    return _uniqueResourceIdentifier;
}

QString NetworkRequest::httpVersion() {
    return _httpVersion;
}

QString NetworkRequest::requestString() {
    return _requestString;
}

QString NetworkRequest::queryString() {
    return _queryString;
}

QStringList NetworkRequest::availableQueryParameters() {
    return _queryParameters.keys();
}

QString NetworkRequest::queryParameter(QString parameter) {
    if(_queryParameters.contains(parameter)) {
        return _queryParameters[parameter];
    } else {
        return "";
    }
}

} // namespace Shark
