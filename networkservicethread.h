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
#include "networkservice.h"
#include "logger.h"

// Qt includes
#include <QThread>
#include <QMutex>

namespace WebServer {

/**
 * @brief The NetworkServiceThread class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class NetworkServiceThread : public QThread, public Logger {
    Q_OBJECT
public:
    /**
     * @brief NetworkServiceThread
     * @param webService
     */
    NetworkServiceThread(NetworkService& webService);

    /**
     * @brief ~NetworkServiceThread
     */
    virtual ~NetworkServiceThread();

    /**
     * @brief The NetworkServiceThreadState enum
     */
    enum NetworkServiceThreadState {
        Idle,
        ProcessingRequest,
        ProcessingResponse
    };

    /**
     * @brief NetworkServiceThreadState
     * @return
     */
    NetworkServiceThreadState networkServiceThreadState();

public slots:
    /**
     * @brief serve
     * @param socketHandle
     */
    void serve(int socketHandle);

private slots:
    /**
     * @brief readClient
     */
    void readClient();

    /**
     * @brief discardClient
     */
    void discardClient();

private:
    /**
     * @brief setNetworkServiceThreadState
     * @param state
     */
    void setNetworkServiceThreadState(NetworkServiceThreadState state);

    NetworkService&             _networkService;
    QMutex                  _networkServiceStateMutex;
    NetworkServiceThreadState   _networkServiceThreadState;
};

} // namespace WebServer

