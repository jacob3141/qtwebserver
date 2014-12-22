//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
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

// Own includes
#include "Application.h"

namespace Shark {

Application::Application(QString rootDirectory)
    : Logger("Shark::Application"), Http::Responder() {
    _rootDirectory = rootDirectory;
}

void Application::respond(NetworkRequest& request, NetworkResponse& response) {
    Q_UNUSED(request);

    response.setBody(QString(HTML(
        <!DOCTYPE html>
        <html>
         <head>
          <title>Shark Web Application Server</title>
         </head>
         <body>
          <p>It works!</p>
         </body>
        </html>
    )));
}

} // namespace Shark
