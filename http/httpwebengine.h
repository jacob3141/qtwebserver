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
#include "httpresource.h"
#include "tcp/tcpresponder.h"
#include "misc/threadsafety.h"

// Qt includes
#include <QObject>
#include <QMap>
#include <QSet>

namespace QtWebServer {

namespace Http {

class WebEngine :
    public QObject,
    public Tcp::Responder {
    Q_OBJECT
public:
    WebEngine(QObject *parent = 0);

    void respond(QSslSocket *sslSocket);
    void addResource(Resource *resource);

private:
    Http::Request acquireSocket(QSslSocket *sslSocket);
    void releaseSocket(QSslSocket *sslSocket);

    bool probeAwaitsSslHandshake(QSslSocket *sslSocket);
    Resource *matchResource(QString uniqueResourceIdentifier);

    QByteArray readFromSocket(QSslSocket *sslSocket);
    void writeToSocket(QSslSocket *sslSocket, QByteArray raw);

    QMap<QSslSocket*, Request> _pendingRequests;
    QSet<Resource*> _resources;

    QMutex _pendingRequestsMutex;
    QMutex _resourcesMutex;
};

}

} // namespace QtWebServer
