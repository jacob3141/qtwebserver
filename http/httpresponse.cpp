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

// Qt includes
#include <QStringBuilder>

// Own includes
#include "httpresponse.h"

namespace QtWebServer {

namespace Http {

Response::Response()
    : Logger("WebServer::Http::Response") {
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

    QByteArray response = "";
    response += QString("HTTP/1.1 %1 %2\r\n")
            .arg(_statusCode)
            .arg(Http::reasonPhrase(_statusCode))
            .toUtf8();
    response += QString("Content-Type: %1; charset=\"%2\"\r\n")
            .arg(_contentType)
            .arg(characterEncodingString)
            .toUtf8();
    response += "\r\n";
    response += _body;

    return response;
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

QByteArray Response::body() {
    return _body;
}

void Response::setBody(QByteArray body) {
    _body = body;
}

} // namespace Http

} // namespace QtWebServer
