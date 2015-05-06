//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with QtWebServer.
// If not, see <http://www.gnu.org/licenses/>.
//
// It is possible to obtain a commercial license of QtWebServer.
// Please contact Jacob Dawid <jacob@omg-it.works>
//

#pragma once

// Own includes
#include "httpheaders.h"
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

    /**
     * Set header value.
     * @param header The HTTP header to be set.
     * @param headerValue The HTTP header's value.
     */
    void setHeader(Header header, QString headerValue);

    /**
     * Set header value.
     * @param headerName The HTTP header to be set.
     * @param headerValue The HTTP header's value.
     */
    void setHeader(QString headerName, QString headerValue);

    /** @returns the value of the specified header. */
    QString header(Header header) const;

    /** @returns the value of the specified header. */
    QString header(QString headerName) const;

private:
    Http::StatusCode        _statusCode;
    QMap<QString, QString>  _headers;
    QByteArray              _body;
};

} // Http

} // namespace QtWebServer
