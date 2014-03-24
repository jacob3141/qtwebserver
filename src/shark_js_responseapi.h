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

#ifndef SHARK_JS_RESPONSEAPI_H
#define SHARK_JS_RESPONSEAPI_H

// Own includes
#include "shark_http_response.h"
#include "shark_engine.h"
#include "shark_logger.h"

// Qt includes
#include <QObject>
#include <QString>
#include <QDomDocument>
#include <QJSValue>

namespace Shark {

namespace Js {

class ResponseAPI : public QObject, public Logger {
    Q_OBJECT
public:
    ResponseAPI(Shark::Engine& engine, Http::Response& response, QObject *parent = 0);
    virtual ~ResponseAPI();

    /**
     * Compiles the response to serialized data.
     */
    void generateBodyFromDOM();

    void addClientSideScript(QString clientSideScript);
public slots:
    /**
     * @returns the dom document.
     */
    QJSValue document();

private:
    QString _responseBody;
    QDomDocument *_responseDomDocument;
    Http::Response& _response;
    Shark::Engine& _engine;

    QDomElement _htmlElement;
    QDomElement _headElement;
    QDomElement _bodyElement;
    QDomElement _scriptElement;
};

} // namespace Js

} // namespace Shark

#endif // SHARK_JS_RESPONSEAPI_H
