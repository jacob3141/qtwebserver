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

#ifndef SHARK_ENGINE_H
#define SHARK_ENGINE_H

// Own includes
#include "shark_application.h"
#include "shark_http_request.h"
#include "shark_http_response.h"
#include "shark_logger.h"

// Qt includes
#include <QJSEngine>
#include <QJSValue>
#include <QString>

namespace Shark {

class Engine : public Logger {
public:
    Engine(Application *application);

    /**
     * Transfers the given QObject to script space. That means, that you will
     * also lose ownership over the object. The script engine automatically will
     * handle deletion at an appropriate time.
     * @param object The Qt object you want to transfer into script space.
     * @return The script value handle to the object in script space.
     */
    QJSValue toJSValue(QObject *object);

    QJSValue createArray();

    bool evaluate(QString program, Http::Request& request, Http::Response& response);

private:
    Application *_application;
    QJSEngine _scriptEngine;
};

} // namespace Shark

#endif // SHARK_ENGINE_H
