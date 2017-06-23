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

/**
 * @class WebEngine
 * @author Jacob Dawid
 * Web engine that reads and writes data to sockets and manages resources.
 */
class WebEngine :
    public QObject,
    public Tcp::Responder {
    Q_OBJECT
public:
    WebEngine(QObject *parent = 0);

    /**
     * Reads available data from sockets and responds properly.
     * @param sslSocket The socket that shall be responded to.
     */
    void respond(QSslSocket *sslSocket);

    /**
     * Registers a new resource.
     * @param resource The resource to be registered.
     */
    void addResource(Resource *resource);

    /**
     * Registers error page themplate
     * @param resource The resource to be registered.
     */
    void addNotFoundPage(Resource *resource);

private:
    /**
     * Acquires a socket and keeps it in an internal list for pending reponses,
     * if the list does not already contain it. This can be required due to
     * several reasons, for example if not all data has been read from the
     * socket yet.
     * @param sslSocket The socket that shall be kept acquired.
     * @returns the actual request.
     */
    Http::Request acquireSocket(QSslSocket *sslSocket);

    /** Releases a socket from the internal list. */
    void releaseSocket(QSslSocket *sslSocket);

    /**
     * Peeks (ie. reads, but does not remove data from the read buffer) the
     * incoming data and tries to determine heuristically, whether the socket
     * awaits an SSL handshake.
     * @param sslSocket The socket to probe.
     * @returns true, when the socket seems to await an SSL handshake.
     */
    bool probeAwaitsSslHandshake(QSslSocket *sslSocket);

    /**
     * Tries to match a resource from the passed unique resource identifier.
     * @param uniqueResourceIdentifier The identifier that shall be matched.
     * @returns the first occurrence of a matched resource.
     */
    Resource *matchResource(QString uniqueResourceIdentifier);

    /**
     * Reads all available data from a socket.
     * @param sslSocket The socket to read from.
     * @returns the data that has been read.
     */
    QByteArray readFromSocket(QSslSocket *sslSocket);

    /**
     * Writes all data to the given socket.
     * @param sslSocket The socket to write to.
     * @param raw The data that shall be written.
     */
    void writeToSocket(QSslSocket *sslSocket, QByteArray raw);

    QMap<QSslSocket*, Request> _pendingRequests;
    QSet<Resource*> _resources;

    QMutex _pendingRequestsMutex;
    QMutex _resourcesMutex;
    Resource* _notFoundPage;
};

}

} // namespace QtWebServer
