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

// Qt includes
#include <QStringBuilder>

// Own includes
#include "NetworkResponse.h"

namespace Shark {

NetworkResponse::NetworkResponse()
    : Logger("Shark::Http::Response") {
    _statusCode = Http::Ok;
    _contentType = "text/html";
    _characterEncoding = Utf8;
    _body = "";
}

QByteArray NetworkResponse::toByteArray() {
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

Http::StatusCode NetworkResponse::statusCode() {
    return _statusCode;
}

void NetworkResponse::setStatusCode(Http::StatusCode statusCode) {
    _statusCode = statusCode;
}

QString NetworkResponse::contentType() {
    return _contentType;
}

void NetworkResponse::setContentType(QString contentType) {
    _contentType = contentType;
}

NetworkResponse::CharacterEncoding NetworkResponse::characterEncoding() {
    return _characterEncoding;
}

void NetworkResponse::setCharacterEncoding(CharacterEncoding characterEncoding) {
    _characterEncoding = characterEncoding;
}

QString NetworkResponse::body() {
    return _body;
}

void NetworkResponse::setBody(QString body) {
    _body = body;
}

} // namespace Shark
