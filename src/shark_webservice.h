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

#ifndef SHARK_SERVICE_H
#define SHARK_SERVICE_H

// Qt includes
#include <QTcpServer>
#include <QVector>

// Own includes
#include "shark_httpresponder.h"

namespace Shark {

class WebServiceThread;

/**
 * @brief The WebService class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class WebService : public QTcpServer {
    Q_OBJECT
public:
    /**
     * @brief WebService
     */
    WebService();

    /**
     * @brief ~WebService
     */
    virtual ~WebService();

    /**
     * @brief initialize
     */
    void initialize();

    /**
     * @brief httpResponder
     * @return
     */
    Http::Responder *httpResponder();

    /**
     * @brief setHttpResponder
     * @param httpResponder
     */
    void setHttpResponder(Http::Responder *httpResponder);

protected:
#if (QT_VERSION < QT_VERSION_CHECK(5, 0, 0))
    /**
     * @brief incomingConnection
     * @param handle
     */
    void incomingConnection(int handle);
#else
    /**
     * @brief incomingConnection
     * @param handle
     */
    void incomingConnection(qintptr handle);
#endif

private:
    Http::Responder *_httpResponder;
    int _port;
    int _threads;

    int _nextRequestDelegatedTo;
    QVector<WebServiceThread*> _webServiceThreads;
};

} // namespace Shark

#endif // SHARK_SERVICE_H
