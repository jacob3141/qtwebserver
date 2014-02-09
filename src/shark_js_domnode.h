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

#ifndef SHARK_JS_DOMNODE_H
#define SHARK_JS_DOMNODE_H

// Own includes
#include "shark_engine.h"

// Qt includes
#include <QObject>
#include <QDomElement>
#include <QDomDocument>

namespace Shark {

namespace Js {

class DomNode : public QObject {
    Q_OBJECT
public:
    DomNode(Shark::Engine& engine, QDomNode domNode, QObject *parent = 0);

public slots:

    // DOM read-only attributes
    QString nodeName();
    QString nodeType();
    QJSValue parentNode();
    QJSValue childNodes();
    QJSValue firstChild();
    QJSValue lastChild();
    QJSValue previousSibling();
    QJSValue nextSibling();
    QJSValue attributes();
    QJSValue ownerDocument();
    QString namespaceURI();
    QString localName();
    bool hasAttributes();

    // Qt extensions
    bool isAttr();
    bool isCDATASection();
    bool isDocumentFragment();
    bool isDocument();
    bool isDocumentType();
    bool isElement();
    bool isEntityReference();
    bool isText();
    bool isEntity();
    bool isNotation();
    bool isProcessingInstruction();
    bool isCharacterData();
    bool isComment();

private:
    Shark::Engine& _engine;
    QDomNode _domNode;
};

} // namespace Js

} // namespace Shark

#endif // SHARK_JS_DOMNODE_H
