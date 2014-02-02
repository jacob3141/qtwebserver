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

#ifndef SHARK_WEBSERVICETHREAD_H
#define SHARK_WEBSERVICETHREAD_H

// Qt includes
#include <QThread>
#include <QMutex>

// Own includes
#include "shark_webservice.h"

namespace Shark {

/**
 * @brief The WebServiceThread class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class WebServiceThread : public QThread {
    Q_OBJECT
public:
    /**
     * @brief WebServiceThread
     * @param webService
     */
    WebServiceThread(WebService& webService);

    /**
     * @brief ~WebServiceThread
     */
    virtual ~WebServiceThread();

    /**
     * @brief The WebServiceThreadState enum
     */
    enum WebServiceThreadState {
        Idle,
        ProcessingRequest,
        ProcessingResponse
    };

    /**
     * @brief webServiceThreadState
     * @return
     */
    WebServiceThreadState webServiceThreadState();

public slots:
    /**
     * @brief serve
     * @param socketHandle
     */
    void serve(int socketHandle);

private slots:
    /**
     * @brief readClient
     */
    void readClient();

    /**
     * @brief discardClient
     */
    void discardClient();

private:
    /**
     * @brief setWebServiceThreadState
     * @param state
     */
    void setWebServiceThreadState(WebServiceThreadState state);

    WebService&             _webService;
    QMutex                  _webServiceStateMutex;
    WebServiceThreadState   _webServiceThreadState;
};

} // namespace Shark

#endif // SHARK_WEBSERVICETHREAD_H
