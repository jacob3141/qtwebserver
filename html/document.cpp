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
#include "document.h"

namespace QtWebServer {

namespace Html {

Document::Document(QString documentTypeDeclaration)
    : QDomDocument() {
    setContent(documentTypeDeclaration);

    _html = createElement("html");
    _head = createElement("head");
    _body = createElement("body");
    QDomElement titleElement = createElement("title");
    _title = createTextNode("Page title");
    titleElement.appendChild(_title);

    _html.appendChild(_head);
    _html.appendChild(_body);
    _head.appendChild(titleElement);
    appendChild(_html);
}

Document::~Document() {
}

void Document::setDocumentTitle(QString title) {
    _title.setNodeValue(title);
}

QString Document::documentTitle() {
    return _title.nodeValue();
}

QDomElement Document::html() {
    return _html;
}

QDomElement Document::head() {
    return _head;
}

QDomElement Document::body() {
    return _body;
}

} // namespace Html

} // namespace QtWebServer
