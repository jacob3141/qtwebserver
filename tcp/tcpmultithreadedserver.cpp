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
#include <QSettings>
#include <QMetaObject>
#include <QTimer>
#include <QSslKey>

// Own includes
#include "tcpmultithreadedserver.h"
#include "tcpserverthread.h"

namespace QtWebServer {

namespace Tcp {

MultithreadedServer::MultithreadedServer()
    : QTcpServer(),
      Logger("WebServer::WebService") {
    setDefaultSslConfiguration();
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

void MultithreadedServer::setSslConfiguration(QSslConfiguration sslConfiguration) {
    _sslConfiguration = sslConfiguration;
}

QSslConfiguration MultithreadedServer::sslConfiguration() const {
    return _sslConfiguration.r();
}

void MultithreadedServer::incomingConnection(qintptr socketDescriptor) {
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

    // Use invokeMethod here to decouple threads
    QMetaObject::invokeMethod(serverThread, "handleNewConnection", Q_ARG(int, socketDescriptor));
}

void MultithreadedServer::setDefaultSslConfiguration() {
    // Set a default SSL configuration just to have it running out of the
    // box. Only for development purposes, never distribute an application
    // that relies on SSL with the default configuration set. You can
    // generate your own key and self-signed certificate:
    //
    // $ openssl req -x509 -newkey rsa:2048 -keyout server.key \
    //               -nodes -days 365 -out server.crt

    QSslConfiguration sslConfiguration;
    QByteArray defaultKey =
        "-----BEGIN PRIVATE KEY-----\n"
        "MIIEvgIBADANBgkqhkiG9w0BAQEFAASCBKgwggSkAgEAAoIBAQDQdRrom1g/MsJh\n"
        "bfwVBZH+oxuGeIBO3jeNWs2bjpDmjtEtMz66LfFVrjuggt0UeNFb1Ve3tYIVq4Wj\n"
        "gmHYUOB44KCzVqJrd1mZdDKukdvoalJyh5f5/kqLdNGzrfjl5fmtK4jy76xM0oV4\n"
        "aZlj8eC4+odrS9HLHfBylDIeSXPWtThFw02IGiQALbtW8cqHSOXhucPAwiNFELVd\n"
        "jjFLa0buQSWq3WSZnroLwOXxfd2dtIoDIn319wTRFPAHWQDqlIuvefYvNzel65yY\n"
        "ZwoomIOrcxfN7IczLxkryD1uZpr4u2aWpHpz+mB7WXhPW82H6qT205e25iDbiZUQ\n"
        "+2UJWBHBAgMBAAECggEBAKF7Fe066ZGLcKio2q4uMnpfP+NbVYnC+qW1wbDPL9Bq\n"
        "sf+hwuXW0SzeW7JrrXc+YHATRHA7WxoOVOflCIbZoAoDeHl0kz1Mp0wIh3pT41aX\n"
        "hmbSQxiVtIlzZT8bdcQh8tgC8YO/xml8a4hrxTd3F+4zpNOpWEtQVgNsaaasn9+h\n"
        "/I/8uQ6oYkNISbnrs7u2fkFAw2/RbyZ6E2Dr9fMiDVUSa5ppFFHjfkgzbGA56Mtg\n"
        "0DdpxWMhICVMwlTI23zmjIAssyJG6xfCQfll9XbsiPAx9/uwAMIFPaUaWBzEUmhh\n"
        "L+/7hESQ7DGIfKxUqtiyvT53EuupxUxkcr0useZY5UECgYEA6wis5xcEo4eBk5o4\n"
        "IYcdZ93LrgwKtWj6e50sVs4NgCgsDWbH99IhNIxwQnCJ02O/aacbFV0d+jB9Jh+M\n"
        "NKTmqDxoe+EY2WJ+sR/YZ9VZwoHFwqubL1BzroX8FBN363tjicfFA+o3bSb4OXDC\n"
        "PINFzSLH8cLhFOo3UFiZf9I9ZZcCgYEA4w2FDGzhnpZPToiT/nwf55Ze2lJsPWlP\n"
        "8XydHoIWDIbUd1LnxBnoGZF1pefexoIy8zesGBfjHC7t6tC/4zy2oqECiP6ESACN\n"
        "8yhSDG+iegTU59LOk+blserR52c2vNGOR7RFbAyZeg3XXwlvQOpaquIP6tc7vkv1\n"
        "FijDcWm3nmcCgYAUAu0Vz/PpKIRz2NGmqSZrzYqmCwhuYb3SAJPh4DFuE/2MNpAY\n"
        "HaAOJVb5kTNq+Dc3+65mp0MCJlbBhDYf1Vp+QIZ05bmD6Mr4sclvLc8yrHH2HT5d\n"
        "TPBMj8PiwbWYKy1ScdvodWy6snK4EU24cUTkLm3vu7QGX9rN0H1hULlBiQKBgQC5\n"
        "rgW/ao98HJoQtFqrBCEnR+6cBkmkUypgZzMqjuGvLtg0GOWWlkUcG7uliKDDoBhA\n"
        "lKe1MFu3YZ2JqVszXyRQjPHAzLurEmEDmFa4+tZZiPf/+YKcq3fubwVngx3dflYn\n"
        "x4H6YbdlfEpD2zhTUxQAqUyxFYEzF3T/wShL6FOUQwKBgA2qSGUvxHcyWw8jnQ2g\n"
        "BbwtU75LCvTrSuIQ8DJIVt5+W9iseNi+q51w6hTAUleJ+4X7RDrSisQucbDAY+Fg\n"
        "Tyz0YVk4+WR0G7ZYAjLJBe7KpzpDZ4mI8yhfYBMhH+Rtkzk96ECezEve8L3llUqD\n"
        "s7+8TR6cFYhHQQH3DQ5BKdt0\n"
        "-----END PRIVATE KEY-----";

    QByteArray defaultCertificate =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDhzCCAm+gAwIBAgIJAP9LmloP2P0tMA0GCSqGSIb3DQEBCwUAMFoxCzAJBgNV\n"
        "BAYTAkRFMRMwEQYDVQQIDApTb21lLVN0YXRlMSAwHgYDVQQKDBdRdFdlYlNlcnZl\n"
        "ciBEZXZlbG9wbWVudDEUMBIGA1UEAwwLRGV2ZWxvcG1lbnQwHhcNMTUwNDIxMDYw\n"
        "MTE4WhcNNDUwNDEzMDYwMTE4WjBaMQswCQYDVQQGEwJERTETMBEGA1UECAwKU29t\n"
        "ZS1TdGF0ZTEgMB4GA1UECgwXUXRXZWJTZXJ2ZXIgRGV2ZWxvcG1lbnQxFDASBgNV\n"
        "BAMMC0RldmVsb3BtZW50MIIBIjANBgkqhkiG9w0BAQEFAAOCAQ8AMIIBCgKCAQEA\n"
        "0HUa6JtYPzLCYW38FQWR/qMbhniATt43jVrNm46Q5o7RLTM+ui3xVa47oILdFHjR\n"
        "W9VXt7WCFauFo4Jh2FDgeOCgs1aia3dZmXQyrpHb6GpScoeX+f5Ki3TRs6345eX5\n"
        "rSuI8u+sTNKFeGmZY/HguPqHa0vRyx3wcpQyHklz1rU4RcNNiBokAC27VvHKh0jl\n"
        "4bnDwMIjRRC1XY4xS2tG7kElqt1kmZ66C8Dl8X3dnbSKAyJ99fcE0RTwB1kA6pSL\n"
        "r3n2Lzc3peucmGcKKJiDq3MXzeyHMy8ZK8g9bmaa+LtmlqR6c/pge1l4T1vNh+qk\n"
        "9tOXtuYg24mVEPtlCVgRwQIDAQABo1AwTjAdBgNVHQ4EFgQUm4DLVsXhMy8J7Us4\n"
        "QoTdTgVS9CEwHwYDVR0jBBgwFoAUm4DLVsXhMy8J7Us4QoTdTgVS9CEwDAYDVR0T\n"
        "BAUwAwEB/zANBgkqhkiG9w0BAQsFAAOCAQEAmxPOPLbq0EBBWrboNfGtD1OTnchy\n"
        "kBlhrSG+2gLNS3lfiphOZ+qtUGNTgewAk+nR5sraw4uFG8+6oQmNFS35zlbzWhgC\n"
        "dveQaT4CNtXCpAevRCbhhuDxzU0W0Dtenf3xFBC2wUWbAwxGZuklyA5ohl9+1By7\n"
        "YG5dPYYsgoeQl9t5yg5orh39kcrrz/exNlPH71qZO6QrPLlaRs0q4S4Yj33hgm/1\n"
        "XAILf9ZtcTElTRhSHC81bZz2HG84nP39WJCe4hH081cdkAMm7W2fzPTzHUWWRW34\n"
        "PMB0Ipp7R7CrztyxyaaDo1S3ozEpqvVT0KI65dWLt8ZsggcKxP6P5aeCBQ==\n"
        "-----END CERTIFICATE-----";

    QSslKey sslKey(defaultKey, QSsl::Rsa);
    sslConfiguration.setPrivateKey(sslKey);

    QSslCertificate sslCertificate(defaultCertificate);
    sslConfiguration.setLocalCertificate(sslCertificate);

    sslConfiguration.setProtocol(QSsl::AnyProtocol);
    setSslConfiguration(sslConfiguration);
}

} // namespace Tcp

} // namespace QtWebServer
