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
#include "multithreadedtcpserver.h"
#include "logger.h"
#include "threadsafety.h"

// Qt includes
#include <QThread>
#include <QMutex>

namespace QtWebServer {

/**
 * @brief The NetworkServiceThread class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class TcpServerThread : public QThread, public Logger {
    friend class MultithreadedTcpServer;
    Q_OBJECT
public:
    /**
     * @brief The NetworkServiceThreadState enum
     */
    enum NetworkServiceThreadState {
        Idle,               /** Thread is idle and waiting to process the next client. */
        ProcessingRequest,  /** Thread is busy reading the request. */
        ProcessingResponse  /** Thread is buy generating a response. */
    };

    virtual ~TcpServerThread();

    /**
     * @brief NetworkServiceThreadState
     * @return
     */
    NetworkServiceThreadState state();

public slots:
    /**
     * @brief serve
     * @param socketHandle
     */
    void serve(int socketHandle);

private slots:
    /**
     * Answers a request from a client.
     */
    void respondToClient();
    void cleanup();

signals:
    void stateChanged(NetworkServiceThreadState state);

private:
    TcpServerThread(MultithreadedTcpServer& multithreadedTcpServer);

    /**
     * @brief setNetworkServiceThreadState
     * @param state
     */
    void setState(NetworkServiceThreadState state);

    MultithreadedTcpServer&     _multithreadedTcpServer;

    ThreadSafe<NetworkServiceThreadState> _networkServiceThreadState;

};

} // namespace QtWebServer

