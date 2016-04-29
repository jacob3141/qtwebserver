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

// Own includes
#include "httpwebengine.h"
#include "httprequest.h"
#include "httpresponse.h"

// Qt includes
#include <QString>
#include <QStringList>
#include <QDebug>

namespace QtWebServer {

namespace Http {

WebEngine::WebEngine(QObject *parent) :
    QObject(parent),
    Responder() {
}

void WebEngine::respond(QSslSocket* sslSocket) {
    // Probe if the client awaits an SSL handshake first before reading any
    // data.
    if(probeAwaitsSslHandshake(sslSocket)) {
        // Do not change the following line for security reasons
        sslSocket->setProtocol(QSsl::TlsV1_2OrLater);
        sslSocket->startServerEncryption();
        return;
    }

    // Acquire the socket so we remember it if we should receive more data for
    // this request later.
    Http::Request httpRequest = acquireSocket(sslSocket);

    // Check if the request is valid and complete.
    if(httpRequest.isValid() && httpRequest.isComplete()) {
        // Create a response object.
        Http::Response httpResponse;

        // Match the unique resource identifier on a resource.
        Resource *resource = matchResource(httpRequest.uniqueResourceIdentifier());
        if(resource != 0) {
            // If we found a resource, let it deliver the response.
            resource->deliver(httpRequest, httpResponse);
        } else {
            // Otherwise generate a 404.
            httpResponse.setStatusCode(NotFound);
        }

        // Write the complete response to the socket.
        writeToSocket(sslSocket, httpResponse.toByteArray());

        // This is kind of weird, but seems to perform a disconnect in
        // opposition to close.
        sslSocket->disconnectFromHost();

        // We're done with this request, so release the corresponding socket.
        releaseSocket(sslSocket);
    }
}

Http::Request WebEngine::acquireSocket(QSslSocket *sslSocket) {
    // The list of pending requests may be accessed from multiple server
    // threads, so we have to make sure to lock properly.
    MutexLocker mutexLocker(_pendingRequestsMutex); Q_UNUSED(mutexLocker);

    Http::Request httpRequest;

    // Check if we have acquired that socket already.
    if(_pendingRequests.contains(sslSocket)) {
        // Get the current request in progress.
        httpRequest = _pendingRequests.value(sslSocket);

        // Append the data from the socket.
        httpRequest.appendBodyData(sslSocket->readAll());

        // Save back the request in case there is more data to come.
        _pendingRequests.insert(sslSocket, httpRequest);
    } else {
        // Create a completely new request object.
        httpRequest = Http::Request(sslSocket->readAll());

        // Save it in the list of pending requests.
        _pendingRequests.insert(sslSocket, httpRequest);
    }
    return httpRequest;
}

void WebEngine::releaseSocket(QSslSocket *sslSocket) {
    // The list of pending requests may be accessed from multiple server
    // threads, so we have to make sure to lock properly.
    MutexLocker mutexLocker(_pendingRequestsMutex); Q_UNUSED(mutexLocker);

    // Remove all requests concerning this socket.
    _pendingRequests.remove(sslSocket);
}

void WebEngine::addResource(Resource *resource) {
    // The list of pending requests may be accessed from multiple server
    // and other threads, so we have to make sure to lock properly.
    MutexLocker mutexLocker(_resourcesMutex); Q_UNUSED(mutexLocker);
    if(resource == 0) {
        return;
    }

    resource->setParent(this);
    _resources.insert(resource);
}

bool WebEngine::probeAwaitsSslHandshake(QSslSocket *sslSocket) {
    // If the connection is already encrypted a handshake doesn't make
    // sense
    if(sslSocket->isEncrypted()) {
        return false;
    }

    // Since there is no way to unambigously tell that the data coming
    // from a client is unencrypted or not, we try to peek the data and
    // see whether we can successfully initiate an SSL handshake.
    // If that also fails, the request is probably broken anyways.
    QByteArray peekBytes = sslSocket->peek(32768);
    Http::Request request(peekBytes);

    // If the data is garbage, it is likely to be encrypted
    return !request.isValid();
}

Resource *WebEngine::matchResource(QString uniqueResourceIdentifier) {
    // The list of pending requests may be accessed from multiple server
    // and other threads, so we have to make sure to lock properly.
    MutexLocker mutexLocker(_resourcesMutex); Q_UNUSED(mutexLocker);

    foreach(Resource *resource, _resources) {
        if(resource->match(uniqueResourceIdentifier)) {
            return resource;
        }
    }

    return 0;
}

QByteArray WebEngine::readFromSocket(QSslSocket *sslSocket) {
    return sslSocket->readAll();
}

void WebEngine::writeToSocket(QSslSocket *sslSocket, QByteArray raw) {
    int bytesWritten = 0;
    int bytesRemaining = 0;
    do {
        bytesWritten = sslSocket->write(raw);
        if(bytesWritten == -1) {
            break;
        }
        raw = raw.right(raw.count() - bytesWritten);
        bytesRemaining = raw.count();
    } while(bytesRemaining > 0);
}

} // namespace Http

} // namespace QtWebServer
