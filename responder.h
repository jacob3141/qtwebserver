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
#include "networkrequest.h"
#include "networkresponse.h"

namespace QtWebServer {

namespace Http {

/**
 * @brief The Responder class
 * @author Jacob Dawid
 * @date 23.11.2013
 */
class Responder {
public:
    /**
     * @brief respond
     * @param request
     * @param response
     */
    virtual void respond(NetworkRequest& request, NetworkResponse& response) {
        Q_UNUSED(request);
        response.setBody(HTML(
            <!DOCTYPE html>
            <html>
             <head>
              <title>QtWebServer Web Application Server</title>
             </head>
             <body>
              <p>There has no responder been set up.</p>
             </body>
            </html>
        ));
    }
};

} // namespace Http

} // namespace WebServer

