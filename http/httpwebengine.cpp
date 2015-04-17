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
    QByteArray rawRequest = sslSocket->readAll();

    Http::Request httpRequest;
    if(_pendingRequests.contains(sslSocket)) {
        httpRequest = _pendingRequests.value(sslSocket);
        httpRequest.appendBodyData(rawRequest);
        _pendingRequests.insert(sslSocket, httpRequest);
    } else {
        httpRequest = Http::Request(rawRequest);
        _pendingRequests.insert(sslSocket, httpRequest);
    }

    if(httpRequest.isComplete()) {
        Http::Response httpResponse;

        Resource *resource = matchResource(httpRequest.uniqueResourceIdentifier());
        if(resource != 0) {
            resource->deliver(httpRequest, httpResponse);
        } else {
            // Resource not found
            httpResponse.setStatusCode(NotFound);
        }

        write(sslSocket, httpResponse.toByteArray());
        sslSocket->close();
        _pendingRequests.remove(sslSocket);
    }
}

void WebEngine::clientHasConnected(QSslSocket* sslSocket) {
    Tcp::Responder::clientHasConnected(sslSocket);
}

void WebEngine::clientHasQuit(QSslSocket* sslSocket) {
    Tcp::Responder::clientHasQuit(sslSocket);
}

void WebEngine::addResource(Resource *resource) {
    MutexLocker mutexLocker(_resourcesMutex); Q_UNUSED(mutexLocker);
    if(resource == 0) {
        return;
    }

    resource->setParent(this);
    _resources.insert(resource);
}

Resource *WebEngine::matchResource(QString uniqueResourceIdentifier) {
    MutexLocker mutexLocker(_resourcesMutex); Q_UNUSED(mutexLocker);
    foreach(Resource *resource, _resources) {
        if(resource->match(uniqueResourceIdentifier)) {
            return resource;
        }
    }
    return 0;
}

} // namespace Http

} // namespace QtWebServer
