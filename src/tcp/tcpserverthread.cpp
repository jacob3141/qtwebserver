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

// Qt includes
#include <QDateTime>
#include <QEventLoop>
#include <QMetaEnum>
#include <QStringList>
#include <QTimer>

// Own includes
#include "tcpserverthread.h"

namespace QtWebServer {

namespace Tcp {

    ServerThread::ServerThread(MultithreadedServer& multithreadedServer)
        : QThread(0)
        , Logger(QString("WebServer:NetworkServiceThread (%1)").arg((long)this))
        , m_multithreadedServer(multithreadedServer)
    {
        m_networkServiceThreadState = NetworkServiceThreadStateIdle;
    }

    ServerThread::~ServerThread()
    {
    }

    ServerThread::NetworkServiceThreadState ServerThread::state()
    {
        return m_networkServiceThreadState.r();
    }

    void ServerThread::setState(ServerThread::NetworkServiceThreadState state)
    {
        m_networkServiceThreadState = state;
        emit stateChanged(state);
    }

    void ServerThread::handleNewConnection(int socketHandle)
    {
        setState(NetworkServiceThreadStateBusy);

        QSslSocket* sslSocket = new QSslSocket(this);
        connect(sslSocket, &QSslSocket::readyRead, this, &ServerThread::clientDataAvailable);
        connect(sslSocket, &QSslSocket::disconnected, this, &ServerThread::clientClosedConnection);

        // Error/informational signals
        connect(sslSocket, &QSslSocket::peerVerifyError, this, &ServerThread::peerVerifyError);
        connect(sslSocket, &QSslSocket::sslErrors, this, &ServerThread::sslErrors);
        connect(sslSocket, &QSslSocket::modeChanged, this, &ServerThread::modeChanged);
        connect(sslSocket, &QSslSocket::encrypted, this, &ServerThread::encrypted);
        connect(sslSocket, &QSslSocket::encryptedBytesWritten, this, &ServerThread::encryptedBytesWritten);

        sslSocket->setSocketDescriptor(socketHandle);
        sslSocket->setSslConfiguration(m_multithreadedServer.sslConfiguration());

        setState(NetworkServiceThreadStateIdle);
    }

    void ServerThread::clientDataAvailable()
    {
        setState(NetworkServiceThreadStateBusy);

        QSslSocket* sslSocket = (QSslSocket*)sender();

        Responder* responder = m_multithreadedServer.responder();
        if (responder) {
            responder->respond(sslSocket);
        }

        setState(NetworkServiceThreadStateIdle);
    }

    void ServerThread::clientClosedConnection()
    {
        setState(NetworkServiceThreadStateBusy);

        QSslSocket* sslSocket = (QSslSocket*)sender();

        sslSocket->close();
        sslSocket->deleteLater();

        setState(NetworkServiceThreadStateIdle);
    }

    void ServerThread::peerVerifyError(const QSslError& error)
    {
        QSslSocket* sslSocket = dynamic_cast<QSslSocket*>(sender());
        if (sslSocket) {
            log(sslSocket->errorString(), Log::Error);
        }

        QMetaEnum metaEnum = QMetaEnum::fromType<QSslError::SslError>();
        QString errorString = metaEnum.valueToKey(error.error());

        log(QString("Socket error: %1 (%2)").arg(errorString).arg((int)error.error()));
    }

    void ServerThread::sslErrors(QList<QSslError> errors)
    {
        foreach (QSslError error, errors) {
            log(error.errorString(), Log::Error);
        }
    }

    void ServerThread::modeChanged(QSslSocket::SslMode mode)
    {
        switch (mode) {
        case QSslSocket::UnencryptedMode:
            log("SSL socket mode changed to unencrypted mode.");
            break;
        case QSslSocket::SslClientMode:
            log("SSL socket mode changed to client mode.");
            break;
        case QSslSocket::SslServerMode:
            log("SSL socket mode changed to server mode.");
            break;
        }
    }

    void ServerThread::encrypted()
    {
        log("SSL Socket entered encrypted state.");
    }

    void ServerThread::encryptedBytesWritten(qint64 bytes)
    {
        log(QString("Encrypted bytes written: %1").arg(bytes));
    }

} // namespace Tcp

} // namespace QtWebServer
