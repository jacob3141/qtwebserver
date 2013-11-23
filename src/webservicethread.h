//
// Copyright 2010-2013 Jacob Dawid <jacob.dawid@cybercatalyst.net>
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

#ifndef WEBSERVICETHREAD_H
#define WEBSERVICETHREAD_H

#include <QThread>
#include <QMutex>

#include "webservice.h"

class WebServiceThread : public QThread {
    Q_OBJECT
public:
    WebServiceThread(WebService& webService);
    virtual ~WebServiceThread();

    enum WebServiceThreadState {
        Idle,
        ProcessingRequest,
        ProcessingResponse
    };

    WebServiceThreadState webServiceThreadState();
public slots:
    void serve(int socketHandle);

private slots:
    void readClient();
    void discardClient();

private:
    void setWebServiceThreadState(WebServiceThreadState state);

    WebService&             _webService;
    QMutex                  _webServiceStateMutex;
    WebServiceThreadState   _webServiceThreadState;
};

#endif // WEBSERVICETHREAD_H
