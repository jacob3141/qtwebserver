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

#pragma once

// Own includes
#include "httpstatuscodes.h"

#include "misc/logger.h"

// Qt includes
#include <QByteArray>
#include <QNetworkReply>

namespace QtWebServer {

namespace Http {
/**
 * @brief The Response class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class Response : public Logger {
public:
    /**
     * @brief Constructor.
     */
    Response();

    /**
     * @brief Converts the response into a byte array. The resulting character
     * encoding is determined by the characterEncoding attribute.
     * @returns The resulting byte array.
     */
    QByteArray toByteArray();

    /**
     * @returns The status code of this response.
     */
    Http::StatusCode statusCode();

    /**
     * @brief Sets the status code for this response. The reason phrase will be
     * added automatically.
     * @param statusCode A HTTP status code.
     */
    void setStatusCode(Http::StatusCode statusCode);

    /**
     * @returns The response body part.
     */
    QByteArray body();

    /**
     * @brief Sets the response body, ie. the actual document transmitted.
     * @param body An HTTP response body.
     */
    void setBody(QByteArray body);

    void setHeader(QString headerName, QString headerValue);

    QString header(QString headerName);

private:
    Http::StatusCode        _statusCode;
    QMap<QString, QString>  _headers;
    QByteArray              _body;
};

} // Http

} // namespace QtWebServer
