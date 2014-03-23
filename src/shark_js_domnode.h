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
#include "shark_logger.h"

// Qt includes
#include <QObject>
#include <QDomElement>
#include <QDomDocument>

namespace Shark {

namespace Js {

class DomNode : public QObject, public Logger {
    Q_OBJECT
public:
    DomNode(Shark::Engine& engine, QDomDocument& domDocument, QDomNode domNode, QObject *parent = 0);

public slots:
    // Creating subelements
    /**
     * Creates a new element node with the specified tag name below this
     * node. The newly created node will be inserted after the last child
     * node.
     * @param tagName The tag name.
     * @returns a reference to the newly created node.
     */
    QJSValue createElementBelow(QString tagName);

    /**
     * Creates a new text node below this node. The newly created node will
     * be inserted after the last child node.
     * @param tagName The tag name.
     * @returns a reference to the newly created node.
     */
    QJSValue createTextBelow(QString data);

    // Creating elements before
    /**
     * Creates a new element node before this node.
     * @param tagName The tag name.
     * @returns a reference to the newly created node.
     */
    QJSValue createElementBefore(QString tagName);

    /**
     * Creates a new text node before this node.
     * @param tagName The tag name.
     * @returns a reference to the newly created node.
     */
    QJSValue createTextBefore(QString data);

    // Creating elements after
    /**
     * Creates a new element node after this node.
     * @param tagName The tag name.
     * @returns a reference to the newly created node.
     */
    QJSValue createElementAfter(QString tagName);

    /**
     * Creates a new text node after this node.
     * @param tagName The tag name.
     * @returns a reference to the newly created node.
     */
    QJSValue createTextAfter(QString data);

    /** @returns all child elements. */
    QJSValue elements();

    /** @returns all child elements with the given tag name. */
    QJSValue elementsByTagName(QString tagName);

    /** @returns all child elements with the given class name. */
    QJSValue elementsByClass(QString className);

    // Classes
    /**
     * If this node is an element, makes sure it has the specified
     * class.
     * @param className The class name.
     * @returns an object referencing the same node.
     */
    QJSValue addClass(QString className);

    /**
     * If this node is an element, makes sure it does not have the
     * the specified class.
     * @param className The class name.
     * @returns an object referencing the same node.
     */
    QJSValue removeClass(QString className);

    /**
     * If this node is an element, makes sure it has the specified
     * class if it does not have it already, otherweise makes sure
     * it does not have the specified class.
     * @param className The class name.
     * @returns an object referencing the same node.
     */
    QJSValue toggleClass(QString className);

    /**
     * Acts like addClass.
     * @param className The class name.
     * @returns an object referencing the same node.
     */
    QJSValue withClass(QString className);

    /** @returns true if this node is an element and has the specified
     * class, false otherwise. */
    bool hasClass(QString className);

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
    QDomDocument& _domDocument;
    QDomNode _domNode;
};

} // namespace Js

} // namespace Shark

#endif // SHARK_JS_DOMNODE_H
