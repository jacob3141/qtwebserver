//
// Copyright 2010-2014 Jacob Dawid <jacob.dawid@cybercatalyst.net>
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

// Qt includes
#include <QSettings>
#include <QMetaObject>

// Own includes
#include "shark_webservice.h"
#include "shark_webservicethread.h"

namespace Shark {

WebService::WebService()
    : QTcpServer(),
      Logger("Shark::WebService") {
}

WebService::~WebService() {
}

void WebService::initialize() {
    QSettings settings("../etc/shark.settings", QSettings::IniFormat);
    settings.beginGroup("service");
    _port = settings.value("port", 1337).toInt();
    _threads = settings.value("threads", 4).toInt();
    settings.endGroup();

    // Mark all active threads for deletion if already running
    foreach(WebServiceThread* webServiceThread, _webServiceThreads) {
        if(webServiceThread) {
            webServiceThread->deleteLater();
        }
    }
    _webServiceThreads.clear();

    // Create the specified number of threads and store them in a set
    int thread = _threads;
    while(thread > 0) {
        WebServiceThread* webServiceThread = new WebServiceThread(*this);
        webServiceThread->start();
        _webServiceThreads.append(webServiceThread);
        thread--;
    }

    _nextRequestDelegatedTo = 0;

    // Listen
    if(listen(QHostAddress::Any, _port)) {
        log(QString("Listening on port %1.").arg(_port));
    } else {
        log(QString("Could not open port %1.").arg(_port), Log::Error);
    }
}

Http::Responder *WebService::httpResponder() {
    return _httpResponder;
}

void WebService::setHttpResponder(Http::Responder *httpResponder) {
    _httpResponder = httpResponder;
}

#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
void WebService::incomingConnection(int handle) {
#else
void WebService::incomingConnection(qintptr handle) {
#endif
    QMetaObject::invokeMethod(_webServiceThreads[_nextRequestDelegatedTo], "serve", Q_ARG(int, handle));
    _nextRequestDelegatedTo++;
    if(_nextRequestDelegatedTo >= _webServiceThreads.size()) {
        _nextRequestDelegatedTo = 0;
    }
}

} // namespace Shark
