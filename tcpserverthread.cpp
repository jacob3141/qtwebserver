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
#include <QNetworkRequest>

// Own includes
#include "tcpserverthread.h"

namespace QtWebServer {

TcpServerThread::TcpServerThread(MultithreadedTcpServer &multithreadedTcpServer)
    : QThread(0),
      Logger(QString("WebServer:NetworkServiceThread (%1)").arg((long)this)),
      _multithreadedTcpServer(multithreadedTcpServer) {
    _networkServiceThreadState = Idle;
}

TcpServerThread::~TcpServerThread() {
}

TcpServerThread::NetworkServiceThreadState TcpServerThread::networkServiceThreadState() {
    _networkServiceStateMutex.lock();
    NetworkServiceThreadState state = _networkServiceThreadState;
    _networkServiceStateMutex.unlock();
    return state;
}

void TcpServerThread::setNetworkServiceThreadState(TcpServerThread::NetworkServiceThreadState state) {
    _networkServiceStateMutex.lock();
    _networkServiceThreadState = state;
    _networkServiceStateMutex.unlock();
}

void TcpServerThread::serve(int socketHandle) {
    QTcpSocket* tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(discardClient()));
    tcpSocket->setSocketDescriptor(socketHandle);
}

void TcpServerThread::readClient() {
    setNetworkServiceThreadState(ProcessingRequest);
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString httpRequest;
    bool requestCompleted = false;

    QTimer requestTimer, responseTimer;
    requestTimer.setTimerType(Qt::PreciseTimer);
    responseTimer.setTimerType(Qt::PreciseTimer);
    const int timeoutInterval = 10000;
    requestTimer.setInterval(timeoutInterval);
    responseTimer.setInterval(timeoutInterval);

    // Request
    requestTimer.start();
    QByteArray text = socket->readAll();
    httpRequest = QString(text);
    requestCompleted = true;

    int requestTimePassed = timeoutInterval - requestTimer.remainingTime();
    log(QString("Received request within %1 ms.").arg(requestTimePassed));

    // Response
    responseTimer.start();
    if(requestCompleted) {
        setNetworkServiceThreadState(ProcessingResponse);
        NetworkRequest request(httpRequest);
        NetworkResponse response;
        _multithreadedTcpServer.httpResponder()->respond(request, response);
        socket->write(response.toByteArray());
        socket->waitForBytesWritten(10000);
    }
    int responseTimePassed = timeoutInterval - responseTimer.remainingTime();
    log(QString("Generated and sent response within %1 ms.").arg(responseTimePassed));

    // Close connection
    socket->close();
    if(socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }

    setNetworkServiceThreadState(Idle);
}

void TcpServerThread::discardClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

} // namespace WebServer
