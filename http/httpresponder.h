//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with QtWebServer.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

// Own includes
#include "tcp/tcpresponder.h"

#include "httprequest.h"
#include "httpresponse.h"

namespace QtWebServer {

namespace Http {

/**
 * @brief The Responder class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class Responder : public Tcp::Responder {
public:
    virtual void respond(const QByteArray& request, QByteArray& response) {
        Http::Request httpRequest(request);
        Http::Response httpResponse;
        respond(httpRequest, httpResponse);
        response = httpResponse.toByteArray();
    }

    virtual void respond(const Http::Request& request, Response& response) = 0;
};

} // namespace Http

} // namespace QtWebServer

