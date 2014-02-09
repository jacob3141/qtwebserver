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

DomNode::DomNode(Engine &engine, QDomNode domNode, QObject *parent)
    : QObject(parent),
      _engine(engine){
    _domNode = domNode;
}

QString DomNode::nodeName() {
    return _domNode.nodeName();
}

QString DomNode::nodeType() {
    switch(_domNode.nodeType()) {
    case QDomNode::ElementNode:
        return "element";
        break;
    case QDomNode::AttributeNode:
        return "attribute";
        break;
    case QDomNode::TextNode:
        return "text";
        break;
    case QDomNode::CDATASectionNode:
        return "cdata-section";
        break;
    case QDomNode::EntityReferenceNode:
        return "entity-reference";
        break;
    case QDomNode::EntityNode:
        return "entity";
        break;
    case QDomNode::ProcessingInstructionNode:
        return "processing-instruction";
        break;
    case QDomNode::CommentNode:
        return "comment";
        break;
    case QDomNode::DocumentNode:
        return "document";
        break;
    case QDomNode::DocumentTypeNode:
        return "document-type";
        break;
    case QDomNode::DocumentFragmentNode:
        return "document-fragment";
        break;
    case QDomNode::NotationNode:
        return "notation";
        break;
    case QDomNode::BaseNode:
        return "base";
        break;
    case QDomNode::CharacterDataNode:
        return "character-data";
        break;
    }
    return "unknown";
}

QJSValue DomNode::parentNode() {
    return _engine.toJSValue(new DomNode(_engine, _domNode.parentNode()));
}

QJSValue DomNode::childNodes() {
    /* TODO: Implement. */
    return QJSValue();
}

QJSValue DomNode::firstChild() {
    return _engine.toJSValue(new DomNode(_engine, _domNode.firstChild()));
}

QJSValue DomNode::lastChild() {
    return _engine.toJSValue(new DomNode(_engine, _domNode.lastChild()));
}

QJSValue DomNode::previousSibling() {
    return _engine.toJSValue(new DomNode(_engine, _domNode.previousSibling()));
}

QJSValue DomNode::nextSibling() {
    return _engine.toJSValue(new DomNode(_engine, _domNode.nextSibling()));
}

QJSValue DomNode::attributes() {
    /* TODO: Implement. */
    return QJSValue();
}

QJSValue DomNode::ownerDocument() {
    return _engine.toJSValue(new DomNode(_engine, _domNode.ownerDocument()));
}

QString DomNode::namespaceURI() {
    return _domNode.namespaceURI();
}

QString DomNode::localName() {
    return _domNode.localName();
}

bool DomNode::hasAttributes() {
    return _domNode.hasAttributes();
}

bool DomNode::isAttr() {
    return _domNode.isAttr();
}

bool DomNode::isCDATASection() {
    return _domNode.isCDATASection();
}

bool DomNode::isDocumentFragment() {
    return _domNode.isDocumentFragment();
}

bool DomNode::isDocument() {
    return _domNode.isDocument();
}

bool DomNode::isDocumentType() {
    return _domNode.isDocumentType();
}

bool DomNode::isElement() {
    return _domNode.isElement();
}

bool DomNode::isEntityReference() {
    return _domNode.isEntityReference();
}

bool DomNode::isText() {
    return _domNode.isText();
}

bool DomNode::isEntity() {
    return _domNode.isEntity();
}

bool DomNode::isNotation() {
    return _domNode.isNotation();
}

bool DomNode::isProcessingInstruction() {
    return _domNode.isProcessingInstruction();
}

bool DomNode::isCharacterData() {
    return _domNode.isCharacterData();
}

bool DomNode::isComment() {
    return _domNode.isComment();
}

} // namespace Js

} // namespace Shark
