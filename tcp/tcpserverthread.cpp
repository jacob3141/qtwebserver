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
#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>
#include <QTimer>

// Own includes
#include "tcpserverthread.h"

namespace QtWebServer {

namespace Tcp {

ServerThread::ServerThread(MultithreadedServer &multithreadedServer)
    : QThread(0),
      Logger(QString("WebServer:NetworkServiceThread (%1)").arg((long)this)),
      _multithreadedServer(multithreadedServer) {
    _networkServiceThreadState = NetworkServiceThreadStateIdle;
}

ServerThread::~ServerThread() {
}

ServerThread::NetworkServiceThreadState ServerThread::state() {
    return _networkServiceThreadState.r();
}

void ServerThread::setState(ServerThread::NetworkServiceThreadState state) {
    _networkServiceThreadState = state;
    emit stateChanged(state);
}

void ServerThread::serve(int socketHandle) {
    QTcpSocket* tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(respondToClient()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(cleanup()));
    tcpSocket->setSocketDescriptor(socketHandle);
}

void ServerThread::respondToClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    setState(NetworkServiceThreadStateBusy);

    const int timeoutMSec = 30000;
    QTimer timer;
    timer.setTimerType(Qt::PreciseTimer);
    timer.setInterval(timeoutMSec);
    timer.start();

    QByteArray response;
    Responder *responder = _multithreadedServer.responder();
    if(responder) {
       responder->respond(socket->readAll(), response);
    }
    socket->write(response);
    socket->waitForBytesWritten(timeoutMSec);
    socket->close();
    socket->deleteLater();

    int timePassed = timeoutMSec - timer.remainingTime();
    timer.stop();

    log(QString("Generated and sent response within %1 ms.").arg(timePassed));

    setState(NetworkServiceThreadStateIdle);
}

void ServerThread::cleanup() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

} // namespace Tcp

} // namespace QtWebServer
