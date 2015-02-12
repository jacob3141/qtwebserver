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

// Own includes
#include "tcpmultithreadedserver.h"
#include "tcpserverthread.h"

namespace QtWebServer {

namespace Tcp {

MultithreadedServer::MultithreadedServer()
    : QTcpServer(),
      Logger("WebServer::WebService") {
}

MultithreadedServer::~MultithreadedServer() {
    close();
}

bool MultithreadedServer::close() {
    if(isListening()) {
        QTcpServer::close();

        // Mark all active threads for deletion if already running
        foreach(ServerThread* networkServiceThread, _serviceThreads) {
            if(networkServiceThread) {
                networkServiceThread->deleteLater();
            }
        }
        _serviceThreads.clear();
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
        _serviceThreads.append(networkServiceThread);
        thread--;
    }

    _nextRequestDelegatedTo = 0;

    // Listen
    return QTcpServer::listen(address, port);
}

int MultithreadedServer::numberOfThreads() {
    return _serviceThreads.size();
}

Responder *MultithreadedServer::responder() {
    return _responder;
}

void MultithreadedServer::setResponder(Responder *responder) {
    _responder = responder;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
void MultithreadedTcpServer::incomingConnection(int handle) {
#else
void MultithreadedServer::incomingConnection(qintptr handle) {
#endif
    QMetaObject::invokeMethod(_serviceThreads[_nextRequestDelegatedTo], "serve", Q_ARG(int, handle));
    _nextRequestDelegatedTo++;
    if(_nextRequestDelegatedTo >= _serviceThreads.size()) {
        _nextRequestDelegatedTo = 0;
    }
}

} // namespace Tcp

} // namespace QtWebServer
