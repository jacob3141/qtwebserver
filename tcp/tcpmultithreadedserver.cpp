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

// Qt includes
#include <QSettings>
#include <QMetaObject>
#include <QTimer>

// Own includes
#include "tcpmultithreadedserver.h"
#include "tcpserverthread.h"

namespace QtWebServer {

namespace Tcp {

MultithreadedServer::MultithreadedServer()
    : QTcpServer(),
      Logger("WebServer::WebService") {
    _serverTimeoutSeconds = 60;
}

MultithreadedServer::~MultithreadedServer() {
    close();
}

bool MultithreadedServer::close() {
    if(isListening()) {
        QTcpServer::close();

        // Mark all active threads for deletion if already running
        foreach(ServerThread* networkServiceThread, _serverThreads) {
            if(networkServiceThread) {
                networkServiceThread->deleteLater();
            }
        }
        _serverThreads.clear();
    }
    return true;
}

bool MultithreadedServer::listen(const QHostAddress &address,
                                    quint16 port,
                                    int numberOfThreads) {
    if(isListening()) {
        return false;
    }

    // Create the specified number of threads and store them in a vector
    int thread = numberOfThreads;
    while(thread > 0) {
        ServerThread* networkServiceThread = new ServerThread(*this);
        networkServiceThread->start();
        _serverThreads.append(networkServiceThread);
        thread--;
    }

    _nextRequestDelegatedTo = 0;

    // Listen
    return QTcpServer::listen(address, port);
}

int MultithreadedServer::numberOfThreads() {
    return _serverThreads.size();
}

int MultithreadedServer::serverTimeoutSeconds() {
    return _serverTimeoutSeconds.r();
}

void MultithreadedServer::setServerTimeoutSeconds(int seconds) {
    _serverTimeoutSeconds = seconds;
}

Responder *MultithreadedServer::responder() {
    return _responder.r();
}

void MultithreadedServer::setResponder(Responder *responder) {
    _responder = responder;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
void MultithreadedTcpServer::incomingConnection(int handle) {
#else
void MultithreadedServer::incomingConnection(qintptr handle) {
#endif
    ServerThread* serverThread = 0;
    ServerThread::NetworkServiceThreadState state;
    QTimer timer;
    timer.start(serverTimeoutSeconds() * 1000);
    do {
        serverThread = _serverThreads[_nextRequestDelegatedTo];
        state = serverThread->state();

        _nextRequestDelegatedTo++;
        if(_nextRequestDelegatedTo >= _serverThreads.size()) {
            _nextRequestDelegatedTo = 0;
        }

        if(state == ServerThread::NetworkServiceThreadStateBusy &&
           timer.remainingTime() <= 0) {
            // Abort on server timeout
            return;
        }
    } while(state == ServerThread::NetworkServiceThreadStateBusy);

    QMetaObject::invokeMethod(serverThread, "serve", Q_ARG(int, handle));
}

} // namespace Tcp

} // namespace QtWebServer
