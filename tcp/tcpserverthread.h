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
#include "tcpmultithreadedserver.h"

#include "misc/logger.h"
#include "misc/threadsafety.h"

// Qt includes
#include <QThread>
#include <QSslError>
#include <QSslSocket>
#include <QList>

namespace QtWebServer {

namespace Tcp {

/**
 * @brief The NetworkServiceThread class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class ServerThread : public QThread, public Logger {
    friend class MultithreadedServer;
    Q_OBJECT
public:
    /**
     * @brief The NetworkServiceThreadState enum
     */
    enum NetworkServiceThreadState {
        NetworkServiceThreadStateIdle, /** Thread is idle and waiting to process the next client. */
        NetworkServiceThreadStateBusy  /** Thread is busy. */
    };

    virtual ~ServerThread();

    /**
     * @brief NetworkServiceThreadState
     * @return
     */
    NetworkServiceThreadState state();

private slots:
    void handleNewConnection(int socketHandle);
    void clientDataAvailable();
    void clientClosedConnection();

    void error(QAbstractSocket::SocketError error);
    void sslErrors(QList<QSslError> errors);
    void modeChanged(QSslSocket::SslMode mode);
    void encrypted();
    void encryptedBytesWritten(qint64 bytes);

signals:
    void stateChanged(NetworkServiceThreadState state);

private:
    ServerThread(MultithreadedServer& multithreadedServer);

    /**
     * @brief setNetworkServiceThreadState
     * @param state
     */
    void setState(NetworkServiceThreadState state);

    MultithreadedServer& _multithreadedServer;

    ThreadGuard<NetworkServiceThreadState> _networkServiceThreadState;
};

} // namespace Tcp

} // namespace QtWebServer

