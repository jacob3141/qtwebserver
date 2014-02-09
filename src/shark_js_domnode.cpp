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
#include "shark_js_domnode.h"

namespace Shark {

namespace Js {

DomNode::DomNode(QDomDocument &domDocument, Engine &engine, QDomNode domNode, QObject *parent)
    : QObject(parent),
      _engine(engine),
      _domDocument(domDocument) {
    _domNode = domNode;
}

QString DomNode::attribute(QString name, QString defaultValue) {
   return _domNode.attribute(name, defaultValue);
}

void DomNode::setAttribute(QString name, QString value) {
    _domNode.setAttribute(name, value);
}

bool DomNode::hasAttribute(QString name) {
    return _domNode.hasAttribute(name);
}

QScriptValue DomNode::parentElement() {
    QDomNode domNode = _domNode.parentNode();
    if(domNode.isElement()) {
        QDomNode parentElement = domNode.toElement();
        DomNode *parentDomElement = new DomNode(_domDocument, _engine, parentElement);
        return _engine.transferToScriptSpace(parentDomElement);
    }
    return QScriptValue();
}

QScriptValue DomNode::elements() {
    QDomNodeList childDomNodeList = _domNode.childNodes();
    QScriptValue childElementList = _engine.createArray();
    for(int index = 0; index < childDomNodeList.length(); index++) {
        QDomNode domNode = childDomNodeList.at(index);
        if(domNode.isElement()) {
            childElementList.setProperty(index,
                _engine.transferToScriptSpace(
                    new DomNode(_domDocument, _engine, domNode.toElement())));
        }
    }
    return childElementList;
}


QScriptValue DomNode::appendElement(QString tagName) {
    QDomNode domElement = _domDocument.createElement(tagName);
    _domNode.appendChild(domElement);
    return _engine.transferToScriptSpace(new DomNode(_domDocument,
                                                        _engine,
                                                        domElement));
}

QScriptValue DomNode::element(QString tagName) {
    QDomNodeList childDomNodeList = _domNode.childNodes();
    for(int index = 0; index < childDomNodeList.length(); index++) {
        QDomNode domNode = childDomNodeList.at(index);
        if(domNode.isElement()) {
            QDomNode domElement = domNode.toElement();
            if(domElement.tagName() == tagName) {
                return _engine.transferToScriptSpace(new DomNode(_domDocument, _engine, domElement));
            }
        }
    }
    return QScriptValue();
}

QScriptValue DomNode::previous() {
    QDomNode domElement = _domNode.previousSiblingElement(_domNode.tagName());
    if(domElement.isNull()) {
        return QScriptValue();
    } else {
        return _engine.transferToScriptSpace(new DomNode(_domDocument, _engine, domElement));
    }
}

QScriptValue DomNode::next() {
    QDomNode domElement = _domNode.nextSiblingElement(_domNode.tagName());
    if(domElement.isNull()) {
        return QScriptValue();
    } else {
        return _engine.transferToScriptSpace(new DomNode(_domDocument, _engine, domElement));
    }
}

} // namespace Js

} // namespace Shark
