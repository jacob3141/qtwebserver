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
#include "shark_js_responseapi.h"
#include "shark_js_domnode.h"

// Qt includes
#include <QJSEngine>

namespace Shark {

namespace Js {

ResponseAPI::ResponseAPI(Shark::Engine& engine, Http::Response &response, QObject *parent)
    : QObject(parent),
      Logger("Shark::Js::ResponseAPI"),
      _response(response),
      _engine(engine) {
    _responseBody = "";
    _responseDomDocument = new QDomDocument("html");

    _htmlElement = _responseDomDocument->createElement("html");
    _responseDomDocument->appendChild(_htmlElement);

    _headElement = _responseDomDocument->createElement("head");
    _htmlElement.appendChild(_headElement);

    _bodyElement = _responseDomDocument->createElement("body");
    _bodyElement.setAttribute("onload", "client_main();");
    _htmlElement.appendChild(_bodyElement);

    _scriptElement = _responseDomDocument->createElement("script");
    _scriptElement.setAttribute("type", "text/javascript");
    _headElement.appendChild(_scriptElement);
}

ResponseAPI::~ResponseAPI() {
    delete _responseDomDocument;
}

void ResponseAPI::generateBodyFromDOM() {
    // Do not confuse the response body with the html body.
    // The body of the response includes the whole html document.
    _response.setBody(_responseDomDocument->toString(2));
}

void ResponseAPI::addClientSideScript(QString clientSideScript) {
    QDomText commentTextBefore = _responseDomDocument->createTextNode("\n//");
    QDomText commentTextAfter = _responseDomDocument->createTextNode("\n");
    QDomCDATASection scriptText = _responseDomDocument->createCDATASection(clientSideScript + "//");
    _scriptElement.appendChild(commentTextBefore);
    _scriptElement.appendChild(scriptText);
    _scriptElement.appendChild(commentTextAfter);
}

QJSValue ResponseAPI::document() {
    return _engine.toJSValue(new DomNode(_engine, *_responseDomDocument, _responseDomDocument->documentElement()));
}

} // namespace Js

} // namespace Shark
