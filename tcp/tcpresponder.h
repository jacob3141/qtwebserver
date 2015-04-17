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

#pragma once

// Qt includes
#include <QSslSocket>
#include <QList>

namespace QtWebServer {

namespace Tcp {

class Responder {
public:
    virtual void respond(QSslSocket* sslSocket) = 0;

    /** Called whenever a client has connected. */
    virtual void clientHasConnected(QSslSocket* sslSocket) {
        _connectedClients.append(sslSocket);
    }

    /** Called whenever a client has quit. */
    virtual void clientHasQuit(QSslSocket* sslSocket) {
        _connectedClients.removeAll(sslSocket);
        sslSocket->deleteLater();
    }

    bool isClientConnected(QSslSocket *sslSocket) {
        return _connectedClients.contains(sslSocket);
    }

    QByteArray read(QSslSocket *sslSocket) {
        return sslSocket->readAll();
    }

    void write(QSslSocket *sslSocket, QByteArray raw) {
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

private:
    QList<QSslSocket*> _connectedClients;
};

} // namespace Tcp

} // namespace QtWebServer

