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

#ifndef SHARK_JS_REQUESTAPI_H
#define SHARK_JS_REQUESTAPI_H

// Own includes
#include "shark_http_request.h"
#include "shark_engine.h"

// Qt includes
#include <QObject>
#include <QString>
#include <QStringList>

namespace Shark {

namespace Js {

class RequestAPI : public QObject {
    Q_OBJECT
public:
    RequestAPI(Shark::Engine& engine, Http::Request& request, QObject *parent = 0);

public slots:
    /**
     * @returns a list of available query parameters.
     */
    QStringList parameters();

    /**
     * @param name The parameter's name that is queried.
     * @returns the parameter value of the specified parameter.
     */
    QString parameter(QString name);

    /**
     * @param name The parameter's name that is queried.
     * @returns true, when the specified parameter has been set.
     */
    bool hasParameter(QString name);

private:
    Http::Request& _request;
    Shark::Engine& _engine;
};

} // namespace Js

} // namespace Shark

#endif // SHARK_JS_REQUESTAPI_H
