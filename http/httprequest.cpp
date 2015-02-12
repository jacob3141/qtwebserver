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
#include "httprequest.h"

// Qt includes
#include <QStringList>

namespace QtWebServer {

namespace Http {

Request::Request(QString requestString)
    : Logger("WebServer::Http::Request") {
    _requestString = requestString;
    _valid = true;
    _method = "";
    _uniqueResourceIdentifier = "";
    _version = "";

    QStringList lines = requestString.split("\r\n");
    if(lines.size() < 1) {
        // If we have zero request lines, something went horribly wrong.
        // Nevertheless, it's better to stop it here.
        _valid = false;
        return;
    }

    QStringList requestLine = lines.at(0).split(QRegExp("\\s+"));

    if(requestLine.size() < 3) {
        // The request line has to contain three strings: The method
        // string, the request uri and the HTTP version. If we were
        // strict, we shouldn't even accept anything larger than four
        // strings, but we're permissive here.
        _valid = false;
        return;
    }

    _method = requestLine.at(0).toLower();

    QStringList splittedURI = requestLine.at(1).split('?', QString::SkipEmptyParts);
    _queryString = "";
    if(splittedURI.size() > 1) {
        _queryString = splittedURI.at(1);
        QStringList queryParameterAssignmentList = _queryString.split('&', QString::SkipEmptyParts);
        foreach(QString queryParameterAssignment, queryParameterAssignmentList) {
            QStringList assignment = queryParameterAssignment.split('=', QString::SkipEmptyParts);
            if(assignment.size() > 0) {
                _parameters[assignment.at(0)] = assignment.at(1);
            }
        }
    }

    _uniqueResourceIdentifier = splittedURI.at(0);

    _version = requestLine.at(2);
}

bool Request::valid() const {
    return _valid;
}

QString Request::method() const {
    return _method;
}

QString Request::uniqueResourceIdentifier() const {
    return _uniqueResourceIdentifier;
}

QString Request::version() const {
    return _version;
}

QString Request::requestString() const {
    return _requestString;
}

QString Request::queryString() const {
    return _queryString;
}

QStringList Request::availableParameters() const {
    return _parameters.keys();
}

QString Request::parameter(QString parameter) const {
    if(_parameters.contains(parameter)) {
        return _parameters[parameter];
    } else {
        return "";
    }
}

} // namespace Http

} // namespace QtWebServer
