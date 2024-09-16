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
#include "httpiodeviceresource.h"

namespace QtWebServer {

namespace Http {

    IODeviceResource::IODeviceResource(QString uniqueIdentifier,
        QIODevice* ioDevice,
        QObject* parent)
        : Resource(uniqueIdentifier, parent)
        , m_ioDevice(ioDevice)
    {
        if (m_ioDevice) {
            m_ioDevice->setParent(this);
        }

        setContentType("text/plain");
    }

    IODeviceResource::~IODeviceResource()
    {
    }

    void IODeviceResource::deliver(const Http::Request& request, Response& response)
    {
        if (!m_ioDevice) {
            return;
        }

        if (request.method() == Method::GET) {
            response.setHeader(Http::ContentType, contentType());

            m_ioDevice->open(QIODevice::ReadOnly);
            if (m_ioDevice->isOpen()) {
                response.setBody(m_ioDevice->readAll());
                m_ioDevice->close();
                response.setStatusCode(Ok);
            } else {
                response.setBody("");
                response.setStatusCode(Forbidden);
            }
        }
    }

} // Http

} // QtWebServer
