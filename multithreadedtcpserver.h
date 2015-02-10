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
#include "responder.h"
#include "logger.h"

// Qt includes
#include <QTcpServer>
#include <QVector>

namespace QtWebServer {

class TcpServerThread;

/**
 * @brief The WebService class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class MultithreadedTcpServer : public QTcpServer, public Logger {
    Q_OBJECT
public:
    /**
     * @brief WebService
     */
    MultithreadedTcpServer();

    /**
     * @brief ~WebService
     */
    virtual ~MultithreadedTcpServer();

    bool close();
    bool listen(const QHostAddress &address = QHostAddress::Any,
                quint16 port = 0,
                int numberOfThreads = 4);

    int numberOfThreads();

    /**
     * @brief httpResponder
     * @return
     */
    Http::Responder *httpResponder();

    /**
     * @brief setHttpResponder
     * @param httpResponder
     */
    void setHttpResponder(Http::Responder *httpResponder);

protected:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    /**
     * @brief incomingConnection
     * @param handle
     */
    void incomingConnection(int handle);
#else
    /**
     * @brief incomingConnection
     * @param handle
     */
    void incomingConnection(qintptr handle);
#endif

private:
    Http::Responder *_httpResponder;

    int _nextRequestDelegatedTo;
    QVector<TcpServerThread*> _serviceThreads;
};

} // namespace WebServer

