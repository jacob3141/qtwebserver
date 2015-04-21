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
#include <QStringList>
#include <QDateTime>
#include <QTimer>
#include <QEventLoop>

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

void ServerThread::handleNewConnection(int socketHandle) {
    setState(NetworkServiceThreadStateBusy);

    QSslSocket* sslSocket = new QSslSocket(this);
    connect(sslSocket, SIGNAL(readyRead()), this, SLOT(clientDataAvailable()));
    connect(sslSocket, SIGNAL(disconnected()), this, SLOT(clientClosedConnection()));

    // Error/informational signals
    connect(sslSocket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(sslSocket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
    connect(sslSocket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(modeChanged(QSslSocket::SslMode)));
    connect(sslSocket, SIGNAL(encrypted()), this, SLOT(encrypted()));
    connect(sslSocket, SIGNAL(encryptedBytesWritten(qint64)), this, SLOT(encryptedBytesWritten(qint64)));

    sslSocket->setSocketDescriptor(socketHandle);
    sslSocket->setSslConfiguration(_multithreadedServer.sslConfiguration());

    setState(NetworkServiceThreadStateIdle);
}


void ServerThread::clientDataAvailable() {
    setState(NetworkServiceThreadStateBusy);

    QSslSocket* sslSocket = (QSslSocket*)sender();

    Responder *responder = _multithreadedServer.responder();
    if(responder) {
        responder->respond(sslSocket);
    }

    setState(NetworkServiceThreadStateIdle);
}

void ServerThread::clientClosedConnection() {
    setState(NetworkServiceThreadStateBusy);

    QSslSocket* sslSocket = (QSslSocket*)sender();

    sslSocket->close();
    sslSocket->deleteLater();

    setState(NetworkServiceThreadStateIdle);
}

void ServerThread::error(QAbstractSocket::SocketError error) {
    QSslSocket* sslSocket = dynamic_cast<QSslSocket*>(sender());
    if(sslSocket) {
        log(sslSocket->errorString(), Log::Error);
    }

    QString errorString = "Unknown error";
    switch(error) {
    case QAbstractSocket::ConnectionRefusedError:
        errorString = "The connection was refused by the peer (or timed out).";
        break;
    case QAbstractSocket::RemoteHostClosedError:
        errorString = "The remote host closed the connection. Note that the client socket (i.e., this socket) will be closed after the remote close notification has been sent.";
        break;
    case QAbstractSocket::HostNotFoundError:
        errorString = "The host address was not found.";
        break;
    case QAbstractSocket::SocketAccessError:
        errorString = "The socket operation failed because the application lacked the required privileges.";
        break;
    case QAbstractSocket::SocketResourceError:
        errorString = "The local system ran out of resources (e.g., too many sockets).";
        break;
    case QAbstractSocket::SocketTimeoutError:
        errorString = "The socket operation timed out.";
        break;
    case QAbstractSocket::DatagramTooLargeError:
        errorString = "The datagram was larger than the operating system's limit (which can be as low as 8192 bytes).";
        break;
    case QAbstractSocket::NetworkError:
        errorString = "An error occurred with the network (e.g., the network cable was accidentally plugged out).";
        break;
    case QAbstractSocket::AddressInUseError:
        errorString = "The address specified to QAbstractSocket::bind() is already in use and was set to be exclusive.";
        break;
    case QAbstractSocket::SocketAddressNotAvailableError:
        errorString = "The address specified to QAbstractSocket::bind() does not belong to the host.";
        break;
    case QAbstractSocket::UnsupportedSocketOperationError:
        errorString = "The requested socket operation is not supported by the local operating system (e.g., lack of IPv6 support).";
        break;
    case QAbstractSocket::UnfinishedSocketOperationError:
        errorString = "Used by QAbstractSocketEngine only, The last operation attempted has not finished yet (still in progress in the background).";
        break;
    case QAbstractSocket::ProxyAuthenticationRequiredError:
        errorString = "The socket is using a proxy, and the proxy requires authentication.";
        break;
    case QAbstractSocket::SslHandshakeFailedError:
        errorString = "The SSL/TLS handshake failed, so the connection was closed.";
        break;
    case QAbstractSocket::ProxyConnectionRefusedError:
        errorString = "Could not contact the proxy server because the connection to that server was denied.";
        break;
    case QAbstractSocket::ProxyConnectionClosedError:
        errorString = "The connection to the proxy server was closed unexpectedly (before the connection to the final peer was established).";
        break;
    case QAbstractSocket::ProxyConnectionTimeoutError:
        errorString = "The connection to the proxy server timed out or the proxy server stopped responding in the authentication phase.";
        break;
    case QAbstractSocket::ProxyNotFoundError:
        errorString = "The proxy address set with setProxy() (or the application proxy) was not found.";
        break;
    case QAbstractSocket::ProxyProtocolError:
        errorString = "The connection negotiation with the proxy server failed, because the response from the proxy server could not be understood.";
        break;
    case QAbstractSocket::OperationError:
        errorString = "An operation was attempted while the socket was in a state that did not permit it.";
        break;
    case QAbstractSocket::SslInternalError:
        errorString = "The SSL library being used reported an internal error. This is probably the result of a bad installation or misconfiguration of the library.";
        break;
    case QAbstractSocket::SslInvalidUserDataError:
        errorString = "Invalid data (certificate, key, cypher, etc.) was provided and its use resulted in an error in the SSL library.";
        break;
    case QAbstractSocket::TemporaryError:
        errorString = "A temporary error occurred (e.g., operation would block and socket is non-blocking).";
        break;
    default:
    case QAbstractSocket::UnknownSocketError:
        errorString = "An unidentified error occurred.";
        break;
    }

    log(QString("Socket error: %1 (%2)").arg(errorString).arg((int)error));
}

void ServerThread::sslErrors(QList<QSslError> errors) {
    foreach(QSslError error, errors) {
        log(error.errorString(), Log::Error);
    }
}

void ServerThread::modeChanged(QSslSocket::SslMode mode) {
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

void ServerThread::encrypted() {
    log("SSL Socket entered encrypted state.");
}

void ServerThread::encryptedBytesWritten(qint64 bytes) {
    log(QString("Encrypted bytes written: %1").arg(bytes));
}

} // namespace Tcp

} // namespace QtWebServer
