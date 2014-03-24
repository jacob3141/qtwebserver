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

Engine::Engine(QString rootDirectory)
    : Logger("Shark::Engine") {
    _resourceCache = new ResourceCache(rootDirectory);
}

QJSValue Engine::toJSValue(QObject *object) {
    return _scriptEngine.newQObject(object);
}

QJSValue Engine::createArray() {
    return _scriptEngine.newArray();
}

void Engine::preprocess(QString& serverSideJs, QString& clientSideJs) {
    // Remove client code in server side JS
    serverSideJs.replace(QRegExp("@clientbegin.*@clientend"), "");

    // Remove server code in client side JS
    clientSideJs.replace(QRegExp("@serverbegin.*@serverend"), "");

    // Expand include statements
    replaceIncludes(serverSideJs);
    replaceIncludes(clientSideJs);

    // Remove comments and whitespace
    serverSideJs.replace(QRegExp("//[^\n]*\n"), "")
                .replace(QRegExp("^\\s*\n"), "");
    clientSideJs.replace(QRegExp("//[^\n]*\n"), "")
                .replace(QRegExp("^\\s*\n"), "");
}

void Engine::replaceIncludes(QString &js) {
    QMap<QString, QString> replacements;
    QRegExp regExp("//\\s*@include\\s+[^\n]*\n");
    int position = 0;
    int matchedLength = 0;
    do {
        position = regExp.indexIn(js, position);
        matchedLength  = regExp.matchedLength();
        if(position > 0) {
            QString includeStatement = js.mid(position, matchedLength);
            int firstBracketPosition = includeStatement.indexOf("{");
            int secondBracketPosition = includeStatement.indexOf("}", firstBracketPosition);

            QString includeURI = "/" + includeStatement.mid(firstBracketPosition + 1,
                secondBracketPosition - firstBracketPosition - 1);

            QString includeResource = _resourceCache->read(includeURI);
            replaceIncludes(includeResource);
            replacements[includeStatement] = includeResource;
            position += matchedLength;
        }
    } while(position > 0);

    QList<QString> keys = replacements.keys();
    foreach(QString includeStatement, keys) {
        js.replace(includeStatement, replacements[includeStatement]);
    }
}

bool Engine::evaluate(QString uri, Http::Request &request, Http::Response &response) {
    QString program = _resourceCache->read(uri);

    // Prepare APIs
    Js::ResponseAPI *responseAPI = new Js::ResponseAPI(*this, response);
    Js::RequestAPI *requestAPI = new Js::RequestAPI(*this, request);

    QString serverSideJs = program;
    QString clientSideJs = program;
    preprocess(serverSideJs, clientSideJs);

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


ResourceCache *Engine::resourceCache() {
    return _resourceCache;
}

}
