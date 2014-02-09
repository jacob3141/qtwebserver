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
    DomNode(QDomDocument& domDocument, Shark::Engine& engine, QDomNode domNode, QObject *parent = 0);

public slots:
    /**
     * Queries the specified attributes of this element.
     * @param name The name of the specified attribute.
     * @param defaultValue Default value in case the attribute does not exist.
     * @returns the value of the specified attribute.
     */
    QString attribute(QString name, QString defaultValue = QString());

    /**
     * Sets the value of the specified attribute.
     * @param name The name of the specified attribute.
     * @param value The target value of the specified attribute.
     */
    void setAttribute(QString name, QString value);

    /**
     * Queries whether the specified attribute does exist.
     * @param name The name of the specified attribute.
     * @returns the value of the specified attribute.
     */
    bool hasAttribute(QString name);

    /**
     * @returns the parent element of this element.
     */
    QScriptValue parentElement();

    /**
     * @returns an array of all child elements.
     */
    QScriptValue elements();

    /**
     * Appends a new child to this node.
     * @param childTagName The tag name of the new child.
     * @return
     */
    QScriptValue appendElement(QString tagName);

    QScriptValue element(QString tagName);

    QScriptValue previous();
    QScriptValue next();

private:
    Shark::Engine& _engine;
    QDomDocument _domDocument;
    QDomNode _domNode;
};

} // namespace Js

} // namespace Shark

#endif // SHARK_JS_DOMNODE_H
