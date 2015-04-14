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

Request::Request(const QByteArray &rawRequest)
    : Logger("WebServer::Http::Request") {
    deserialize(rawRequest);
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

QString Request::queryString() const {
    return _queryString;
}

QMap<QString, QString> Request::parameters() const {
    return _parameters;
}

QMap<QString, QString> Request::headers() const {
    return _headers;
}

QByteArray Request::body() const {
    return _body;
}

QByteArray Request::takeLine(QByteArray& rawRequest) {
    // Lines in the HTTP protocol are defined to be separated by '\r\n'
    QByteArray line;
    int rawSize = rawRequest.size();
    int rawPosition;

    // Watch out for the first occurrence of '\r'
    for(rawPosition = 0; rawPosition < rawSize - 1; rawPosition++) {
        if(rawRequest[rawPosition] == '\r' && rawRequest[rawPosition + 1] == '\n') {
            break;
        }
    }

    // If there is just two characters left, this must be EOF
    if(rawPosition != rawSize - 2) {
        // Get the line contents
        line = rawRequest.left(rawPosition);

        // Get the remaining data, skipping '\r\n'
        rawRequest = rawRequest.right(rawSize - line.count() - 2);
    } else {
        // EOF
        rawRequest = "";
        line = rawRequest;
    }

    return line;
}

void Request::deserialize(QByteArray rawRequest) {
    log(QString("Deserializing request, %1 bytes").arg(rawRequest.count()));

    _headers.clear();
    _parameters.clear();
    _valid = true;
    _method = "";
    _uniqueResourceIdentifier = "";
    _version = "";

    // Read ahead the first line in the request
    QByteArray rawRequestLine = takeLine(rawRequest);
    QStringList requestLine = QString::fromUtf8(rawRequestLine)
                                .split(QRegExp("\\s+"));

    if(requestLine.size() < 3) {
        // The request line has to contain three strings: The method
        // string, the request uri and the HTTP version. If we were
        // strict, we shouldn't even accept anything larger than four
        // strings, but we're permissive here.
        _valid = false;
        log(QString("Invalid request line: %1 parts").arg(requestLine.count()));
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
            if(assignment.size() > 1) {
                _parameters[assignment.at(0)] = assignment.at(1);
            }
        }
    }

    _uniqueResourceIdentifier = splittedURI.at(0);

    _version = requestLine.at(2);

    QByteArray nextLine;
    while(!(nextLine = takeLine(rawRequest)).isEmpty()) {
        deserializeHeader(nextLine);
    }

    // By definition, all that follows after a \r\n\r\n is the body of the request.
    _body = QByteArray(rawRequest);
}

void Request::deserializeHeader(const QByteArray& rawHeader) {
    QString headerLine = QString::fromUtf8(rawHeader);
    int colonPosition;
    int headerLineLength = headerLine.count();
    for(colonPosition = 0; colonPosition < headerLineLength; colonPosition++) {
        if(headerLine.at(colonPosition) == ':') {
            break;
        }
    }

    if(colonPosition == headerLineLength) {
        log(QString("Invalid header line found %1").arg(headerLine), Log::Warning);
        return;
    }

    QString headerName = headerLine.left(colonPosition);
    QString headerValue = headerLine.right(headerLineLength - colonPosition).trimmed();
    _headers.insert(headerName, headerValue);
}

} // namespace Http

} // namespace QtWebServer
