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
#include "misc/logger.h"

// Qt includes
#include <QString>
#include <QMap>

namespace QtWebServer {

namespace Http {

class Request : public Logger {
public:
    Request(QString requestString);

    /**
     * This method is used to indicate whether a request could not be
     * interpreted correctly. You are supposed to check this first before
     * using any other methods.
     * @returns true, if the request has been valid, false otherwise.
     */
    bool valid() const;

    /** @returns the method for this request. */
    QString method() const;

    /** @returns the URI for this request. */
    QString uniqueResourceIdentifier() const;

    /** @returns the HTTP version for this request. */
    QString version() const;

    /** returns the original request string. */
    QString requestString() const;

    QString queryString() const;

    QStringList availableParameters() const;

    QString parameter(QString parameter) const;

private:
    QString _requestString;
    QString _queryString;
    QString _fragment;
    QString _method;
    QString _uniqueResourceIdentifier;
    QString _version;
    bool _valid;
    QMap<QString, QString> _parameters;
};

}

} // namespace QtWebServer

