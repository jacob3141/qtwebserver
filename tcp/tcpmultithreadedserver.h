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
#include "tcpresponder.h"

#include "misc/logger.h"
#include "misc/threadsafety.h"

// Qt includes
#include <QTcpServer>
#include <QVector>
#include <QSslConfiguration>

namespace QtWebServer {

namespace Tcp {

class ServerThread;

/**
 * @brief The WebService class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class MultithreadedServer : public QTcpServer, public Logger {
    Q_OBJECT
public:
    /** @brief WebService */
    MultithreadedServer();

    /** @brief ~WebService */
    virtual ~MultithreadedServer();

    /** Closes the server immediately. */
    bool close();

    /**
     * Listens to the given address and port.
     * @param address The server address.
     * @param port The server port.
     * @param numberOfThreads The number of threads this server is using.
     * @returns true, if the server is listening.
     */
    bool listen(const QHostAddress &address = QHostAddress::Any,
                quint16 port = 0,
                int numberOfThreads = 4);

    /** Sets the number of threads this server owns. */
    int numberOfThreads();

    /** @returns the server timeout in seconds. */
    int serverTimeoutSeconds();

    /**
     * Sets the server timeout in seconds. If the server is not able
     * to serve the cliend in the given amount of time (ie. all threads
     * are busy for the given amount of seconds) it will simply ignore
     * incoming connections.
     */
    void setServerTimeoutSeconds(int seconds);

    /** @returns the responder for this server. */
    Responder *responder();

    /** Sets the responder for this server. */
    void setResponder(Responder *responder);

    void setSslConfiguration(QSslConfiguration sslConfiguration);
    QSslConfiguration sslConfiguration() const;

protected:
    /**
     * @brief incomingConnection
     * @param handle
     */
    void incomingConnection(qintptr socketDescriptor);

private:
    void setDefaultSslConfiguration();

    ThreadGuard<Responder*> _responder;
    ThreadGuard<int> _serverTimeoutSeconds;

    // Scheduler
    int _nextRequestDelegatedTo;
    QVector<ServerThread*> _serverThreads;

    ThreadGuard<QSslConfiguration> _sslConfiguration;
};

} // namespace Tcp

} // namespace QtWebServer

