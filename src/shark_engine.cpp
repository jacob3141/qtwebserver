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

// Own includes
#include "shark_engine.h"
#include "shark_js_responseapi.h"
#include "shark_js_requestapi.h"

// Qt includes
#include <QStringList>

namespace Shark {

Engine::Engine(Application *application)
    : Logger("Shark::Engine") {
    _application = application;
}

QJSValue Engine::toJSValue(QObject *object) {
    return _scriptEngine.newQObject(object);
}

QJSValue Engine::createArray() {
    return _scriptEngine.newArray();
}

bool Engine::evaluate(QString program, Http::Request &request, Http::Response &response) {
    // Prepare APIs
    Js::ResponseAPI *responseAPI = new Js::ResponseAPI(*this, response);
    Js::RequestAPI *requestAPI = new Js::RequestAPI(*this, request);

    QString serverSideJs = program;
    QString clientSideJs = program;

    serverSideJs.replace(QRegExp("@clientbegin.*@clientend"), "")
                .replace(QRegExp("//[^\n]*\n"), "")
                .replace(QRegExp("^\\s*\n"), "");
    clientSideJs.replace(QRegExp("@serverbegin.*@serverend"), "")
                .replace(QRegExp("//[^\n]*\n"), "")
                .replace(QRegExp("^\\s*\n"), "");

    // Dive into JS space and evaluate
    QJSValueList arguments;
    arguments.append(toJSValue(requestAPI));
    arguments.append(toJSValue(responseAPI));
    QJSValue result = _scriptEngine.evaluate(serverSideJs);

    // Call the server side main function
    _scriptEngine.globalObject().property("server_main").call(arguments);

    if(result.isError()) {
        log(QString("Evaluated JS with return value \"%1\".").arg(result.toString(), Log::Error));
    }

    // Add the client side script
    responseAPI->addClientSideScript("\n" + clientSideJs);

    // Serialize the DOM and set it as the body of the response
    responseAPI->generateBodyFromDOM();

    // Mark objects
    responseAPI->deleteLater();
    requestAPI->deleteLater();
    return !result.isError();
}

}
