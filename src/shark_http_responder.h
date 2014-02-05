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

#ifndef SHARK_HTTP_RESPONDER_H
#define SHARK_HTTP_RESPONDER_H

// Own includes
#include "shark_http_request.h"
#include "shark_http_response.h"

namespace Shark {

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
    virtual void respond(Request& request, Response& response) {
        Q_UNUSED(request);
        response.setBody(HTML(
            <!DOCTYPE html>
            <html>
             <head>
              <title>Shark Web Application Server</title>
             </head>
             <body>
              <p>There has no responder been set up.</p>
             </body>
            </html>
        ));
    }
};

} // namespace Http

} // namespace Shark

#endif // SHARK_HTTP_RESPONDER_H
