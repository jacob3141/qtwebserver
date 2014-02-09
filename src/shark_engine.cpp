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

Engine::Engine(Application *application) {
    _application = application;
}

QJSValue Engine::transferToScriptSpace(QObject *object) {
    return _scriptEngine.newQObject(object);
}

QJSValue Engine::createArray() {
    return _scriptEngine.newArray();
}

bool Engine::evaluate(QString program, Http::Request &request, Http::Response &response) {
    Js::ResponseAPI *responseAPI = new Js::ResponseAPI(*this, response);
    Js::RequestAPI *requestAPI = new Js::RequestAPI(*this, request);

    QJSValue responseJsObject = _scriptEngine.newQObject(responseAPI);
    QJSValue requestJsObject = _scriptEngine.newQObject(requestAPI);

    _scriptEngine.globalObject().setProperty("response", responseJsObject);
    _scriptEngine.globalObject().setProperty("request", requestJsObject);

    QJSValue result = _scriptEngine.evaluate(program);
    responseAPI->compile();

    responseAPI->deleteLater();
    requestAPI->deleteLater();

    return !result.isError();
}

}
