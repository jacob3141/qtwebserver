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
#include "multithreadedtcpserver.h"
#include "tcpserverthread.h"

namespace QtWebServer {

MultithreadedTcpServer::MultithreadedTcpServer()
    : QTcpServer(),
      Logger("WebServer::WebService") {
    _httpResponder = new Http::Responder();
}

MultithreadedTcpServer::~MultithreadedTcpServer() {
    close();
}

bool MultithreadedTcpServer::close() {
    if(isListening()) {
        QTcpServer::close();

        // Mark all active threads for deletion if already running
        foreach(TcpServerThread* networkServiceThread, _serviceThreads) {
            if(networkServiceThread) {
                networkServiceThread->deleteLater();
            }
        }
        _serviceThreads.clear();
    }
    return true;
}

bool MultithreadedTcpServer::listen(const QHostAddress &address,
                                    quint16 port,
                                    int numberOfThreads) {
    if(isListening()) {
        return false;
    }

    // Create the specified number of threads and store them in a vector
    int thread = numberOfThreads;
    while(thread > 0) {
        TcpServerThread* networkServiceThread = new TcpServerThread(*this);
        networkServiceThread->start();
        _serviceThreads.append(networkServiceThread);
        thread--;
    }

    _nextRequestDelegatedTo = 0;

    // Listen
    return QTcpServer::listen(address, port);
}

int MultithreadedTcpServer::numberOfThreads() {
    return _serviceThreads.size();
}

Http::Responder *MultithreadedTcpServer::httpResponder() {
    return _httpResponder;
}

void MultithreadedTcpServer::setHttpResponder(Http::Responder *httpResponder) {
    _httpResponder = httpResponder;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
void WebService::incomingConnection(int handle) {
#else
void MultithreadedTcpServer::incomingConnection(qintptr handle) {
#endif
    QMetaObject::invokeMethod(_serviceThreads[_nextRequestDelegatedTo], "serve", Q_ARG(int, handle));
    _nextRequestDelegatedTo++;
    if(_nextRequestDelegatedTo >= _serviceThreads.size()) {
        _nextRequestDelegatedTo = 0;
    }
}

} // namespace WebServer
