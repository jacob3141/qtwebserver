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
#include "logger.h"

// Qt includes
#include <QString>
#include <QMap>

namespace QtWebServer {

/**
 * @brief The Request class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class NetworkRequest : public Logger {
public:
    NetworkRequest(QString requestString);

    /**
     * This method is used to indicate whether a request could not be
     * interpreted correctly. You are supposed to check this first before
     * using any other methods.
     * @returns true, if the request has been valid, false otherwise.
     */
    bool validRequest();

    /**
     * @returns the method for this request.
     */
    QString method();

    /**
     * @returns the URI for this request.
     */
    QString uniqueResourceIdentifier();

    /**
     * @returns the HTTP version for this request.
     */
    QString httpVersion();

    /**
     * @returns the original request string.
     */
    QString requestString();

    QString queryString();

    QStringList availableQueryParameters();

    QString queryParameter(QString parameter);

private:
    QString _requestString;
    QString _queryString;
    QString _fragment;
    QString _method;
    QString _uniqueResourceIdentifier;
    QString _httpVersion;
    bool _validRequest;
    QMap<QString, QString> _queryParameters;
};

} // namespace WebServer

