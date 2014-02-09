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
      _response(response),
      _engine(engine) {
    _responseBody = "";
    _responseDomDocument = new QDomDocument("html");
    QDomNode htmlElement = _responseDomDocument->createElement("html");
    _responseDomDocument->appendChild(htmlElement);

    QDomNode headElement = _responseDomDocument->createElement("head");
    htmlElement.appendChild(headElement);

    QDomNode bodyElement = _responseDomDocument->createElement("body");
    htmlElement.appendChild(bodyElement);
}

ResponseAPI::~ResponseAPI() {
    delete _responseDomDocument;
}

void ResponseAPI::compile() {
    // Do not confuse the response body with the html body.
    // The body of the response includes the whole html document.
    _response.setBody(_responseDomDocument->toString(4));
}

QJSValue ResponseAPI::document() {
    DomNode *domElement = new DomNode(*_responseDomDocument,
                                      _engine,
                                      _responseDomDocument->documentElement());
    return _engine.transferToScriptSpace(domElement);
}

} // namespace Js

} // namespace Shark
