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
    QSslSocket* socket = new QSslSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(respondToClient()));
    connect(socket, SIGNAL(disconnected()), this, SLOT(cleanup()));
    connect(socket, SIGNAL(error(QAbstractSocket::SocketError)), this, SLOT(error(QAbstractSocket::SocketError)));
    connect(socket, SIGNAL(sslErrors(QList<QSslError>)), this, SLOT(sslErrors(QList<QSslError>)));
    connect(socket, SIGNAL(modeChanged(QSslSocket::SslMode)), this, SLOT(modeChanged(QSslSocket::SslMode)));
    socket->setSocketDescriptor(socketHandle);
}

void ServerThread::error(QAbstractSocket::SocketError error) {
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

    log(QString("Socker error: %1 (%2)").arg(errorString).arg((int)error));
}

void ServerThread::respondToClient() {
    QSslSocket* socket = (QSslSocket*)sender();
    setState(NetworkServiceThreadStateBusy);

    Responder *responder = _multithreadedServer.responder();
    if(responder) {
        QByteArray request = socket->readAll();
        QByteArray response;
        responder->respond(request, response);

        int bytesWritten = 0;
        int bytesRemaining = 0;
        do {
            bytesWritten = socket->write(response);
            if(bytesWritten == -1) {
                break;
            }
            response = response.right(response.count() - bytesWritten);
            bytesRemaining = response.count();
        } while(bytesRemaining > 0);
    }

    socket->close();

    setState(NetworkServiceThreadStateIdle);
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

void ServerThread::cleanup() {
    QSslSocket* socket = (QSslSocket*)sender();
    socket->deleteLater();
}

} // namespace Tcp

} // namespace QtWebServer
