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

#include "misc/logger.h"

// Qt includes
#include <QString>
#include <QMap>

namespace QtWebServer {

namespace Http {

/**
 * @class Request
 * @author Jacob Dawid
 * Basic HTTP request class. Parses a raw HTTP request and splits it into its
 * single components.
 */
class Request :
    public Logger {
public:
    Request();
    Request(const QByteArray& rawRequest);

    /**
     * This method is used to indicate whether a request could not be
     * interpreted correctly. You are supposed to check this first before
     * using any other methods.
     * @returns true, if the request has been valid, false otherwise.
     */
    bool isValid() const;

    /** @returns the method for this request. */
    QString method() const;

    /** @returns the URI for this request. */
    QString uniqueResourceIdentifier() const;

    /** @returns the HTTP version for this request. */
    QString version() const;

    /** @returns url parameters. */
    QMap<QString, QByteArray> urlParameters() const;

    /** @returns a map of request headers (name and value). **/
    QMap<QString, QString> headers() const;

    QString header(Header header) const;
    QString header(QString headerName) const;

    /** @returns the body of the request. */
    QByteArray body() const;

    void appendBodyData(QByteArray bodyData);

    /**
     * Determines whether the request is complete either based
     * on the content length or when all chunks have been transmitted
     */
    bool isComplete() const;

private:
    void setDefaults();
    void deserialize(QByteArray rawRequest);
    void deserializeHeader(const QByteArray& rawHeader);
    QByteArray takeLine(QByteArray& rawRequest);

    QByteArray _body;
    QString _method;
    QString _uniqueResourceIdentifier;
    QString _version;
    bool _valid;
    QMap<QString, QByteArray> _urlParameters;
    QMap<QString, QString> _headers;
};

}

} // namespace QtWebServer

