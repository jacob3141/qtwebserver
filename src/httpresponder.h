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

#ifndef HTTPRESPONDER_H
#define HTTPRESPONDER_H

#include "httprequest.h"
#include "httpresponse.h"

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
            <html>
             <head>
              <title>Shark default responder page</title>
             </head>
             <body>
              <p>This is the default responder message.</p>
             </body>
            </html>
        ));
    }
};

} // namespace Http

#endif // HTTPRESPONDER_H
