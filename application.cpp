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

// Own includes
#include "application.h"

namespace WebServer {

Application::Application(QString rootDirectory)
    : Logger("WebServer::Application"), Http::Responder() {
    _rootDirectory = rootDirectory;
}

void Application::respond(NetworkRequest& request, NetworkResponse& response) {
    Q_UNUSED(request);

    response.setBody(QString(HTML(
        <!DOCTYPE html>
        <html>
         <head>
          <title>WebServer Web Application Server</title>
         </head>
         <body>
          <p>It works!</p>
         </body>
        </html>
    )));
}

} // namespace WebServer
