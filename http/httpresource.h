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
#include "misc/threadsafety.h"
#include "httprequest.h"
#include "httpresponse.h"

// Qt includes
#include <QObject>
#include <QString>

namespace QtWebServer {

namespace Http {

class Resource :
    public QObject {
    Q_OBJECT
public:
    Resource(QString uniqueIdentifier,
             QObject *parent = 0);

    /**
     * Resource matching method. The default implementation matches
     * the unique identifier against a template that allows variable uris.
     *
     * @attention: This method may be called from multiple threads. You are
     * not allowed to perform any operations that are not threadsafe.
     */
    virtual bool match(QString uniqueIdentifier);

    QMap<QString, QString> uriParameters(QString uniqueIdentifier);

    /**
     * The default uri matcher ignores uri parts embraced with "{" and "}",
     * so you can refer to them as uri paramters later. For example:
     *  /service/{account}/{id}
     * will match
     *  /service/user1/23
     *  /service/user4/29
     * @returns the unique resource identifier for this resource.
     */
    QString uniqueIdentifier();

    /**
     * Sets the unique resource identifier for this resource.
     * @param uniqueIdentifer
     */
    void setUniqueIdentifier(QString uniqueIdentifer);

    QString contentType();

    void setContentType(QString contentType);

    virtual void deliver(const Http::Request& request, Http::Response& response) = 0;

private:
    ThreadGuard<QString> _uniqueIdentifier;
    ThreadGuard<QString> _contentType;
};

} // namespace Http

} // namespace QtWebServer
