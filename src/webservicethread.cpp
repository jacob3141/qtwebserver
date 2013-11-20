//
// Copyright 2010-2013 Jacob Dawid <jacob.dawid@cybercatalyst.net>
//
// This file is part of Shark.
//
// Shark is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shark is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shark.  If not, see <http://www.gnu.org/licenses/>.
//

#include "webservicethread.h"

#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>
#include <QTimer>

#include <iostream>

WebServiceThread::WebServiceThread(QObject *parent)
    : QThread(parent) {
    _webServiceState = Idle;
}

WebServiceThread::~WebServiceThread() {
}

WebServiceThread::WebServiceState WebServiceThread::webServiceState() {
    _webServiceStateMutex.lock();
    WebServiceState state = _webServiceState;
    _webServiceStateMutex.unlock();
    return state;
}

void WebServiceThread::setWebServiceState(WebServiceThread::WebServiceState state) {
    _webServiceStateMutex.lock();
    _webServiceState = state;
    _webServiceStateMutex.unlock();
}

void WebServiceThread::serve(int socketHandle) {
    QTcpSocket* tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(discardClient()));
    tcpSocket->setSocketDescriptor(socketHandle);
}

void WebServiceThread::readClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    QString httpRequest;
    bool requestCompleted = false;
    QTimer requestTimer;
    requestTimer.setInterval(10000);
    requestTimer.start();

    while(requestTimer.remainingTime() > 0) {
        if(socket->canReadLine()) {
            QString line = socket->readLine();
            httpRequest.append(line);
            if(line == "\r\n") {
                requestCompleted = true;
                break;
            }
        }
    }

    if(requestCompleted) {
        QTextStream os(socket);
        os.setAutoDetectUnicode(true);
        os << "HTTP/1.0 200 Ok\r\n"
        "Content-Type: text/html; charset=\"utf-8\"\r\n"
        "\r\n"
        "<h1>Nothing to see here</h1>\n"
        << QDateTime::currentDateTime().toString() << "\n";
    }

    socket->close();
    if (socket->state() == QTcpSocket::UnconnectedState) {
        delete socket;
    }
}

void WebServiceThread::discardClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
