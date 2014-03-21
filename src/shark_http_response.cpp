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

// Qt includes
#include <QStringBuilder>

// Own includes
#include "shark_http_response.h"

namespace Shark {

namespace Http {

Response::Response()
    : Logger("Shark::Http::Response") {
    _statusCode = Http::Ok;
    _contentType = "text/html";
    _characterEncoding = Utf8;
    _body = "";
}

QByteArray Response::toByteArray() {
    QString characterEncodingString = "";
    switch(_characterEncoding) {
        case Utf8: characterEncodingString = "utf-8"; break;
    }

    QString response = "";
    response += QString("HTTP/1.1 %1 %2\r\n").arg(_statusCode).arg(Http::reasonPhrase(_statusCode));
    response += QString("Content-Type: %1; charset=\"%2\"\r\n").arg(_contentType).arg(characterEncodingString);
    response += "\r\n";
    response += _body;

    switch(_characterEncoding) {
        default:
        case Utf8: return response.toUtf8();
    }
}

Http::StatusCode Response::statusCode() {
    return _statusCode;
}

void Response::setStatusCode(Http::StatusCode statusCode) {
    _statusCode = statusCode;
}

QString Response::contentType() {
    return _contentType;
}

void Response::setContentType(QString contentType) {
    _contentType = contentType;
}

Response::CharacterEncoding Response::characterEncoding() {
    return _characterEncoding;
}

void Response::setCharacterEncoding(CharacterEncoding characterEncoding) {
    _characterEncoding = characterEncoding;
}

QString Response::body() {
    return _body;
}

void Response::setBody(QString body) {
    _body = body;
}

} // namespace Http

} // namespace Shark
