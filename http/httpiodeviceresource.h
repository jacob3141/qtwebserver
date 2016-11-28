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

// Own includes
#include "httpresource.h"

// Qt includes
#include <QIODevice>

namespace QtWebServer {

namespace Http {

/**
 * @class IODeviceResource
 * @author Jacob Dawid
 * Links a resource with an io device, for example a QFile.
 */
class IODeviceResource :
    public Resource {
    Q_OBJECT
public:
    IODeviceResource(QString uniqueIdentifier,
                     QIODevice *ioDevice,
                     QObject *parent = 0);
    ~IODeviceResource();

    virtual void deliver(const Request& request, Response& response);

private:
    QIODevice *_ioDevice;
};

} // Http

} // QtWebServer
