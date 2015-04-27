//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with QtWebServer.
// If not, see <http://www.gnu.org/licenses/>.
//
// It is possible to obtain a commercial license of QtWebServer.
// Please contact Jacob Dawid <jacob@omg-it.works>
//

// Own includes
#include "cssruleset.h"

// Qt includes
#include <QStringList>

namespace QtWebServer {

namespace Css {

RuleSet::RuleSet() :
    _element("*") {
}

RuleSet::RuleSet(QString block,
                 QString element,
                 QString modifier,
                 bool isWrapper) :
    _block(block),
    _element(element),
    _modifier(modifier),
    _isWrapper(isWrapper) {
}

RuleSet::~RuleSet() {
}

QString RuleSet::block() {
    return _block;
}

void RuleSet::setBlock(QString block) {
    _block = block;
}

QString RuleSet::element() {
    return _element;
}

void RuleSet::setElement(QString selector) {
    _element = selector.trimmed();
}

QString RuleSet::modifier() {
    return _modifier;
}

void RuleSet::setModifier(QString modifier) {
    _modifier = modifier;
}

bool RuleSet::isWrapper() {
    return _isWrapper;
}

void RuleSet::setWrapper(bool isWrapper) {
    _isWrapper = isWrapper;
}

void RuleSet::addDeclaration(QString property,
                             QString value) {
    _declarations.insert(property, value);
}

void RuleSet::addDeclarations(QString declarations) {
    QStringList splitDeclarations = declarations.split(";", QString::SkipEmptyParts);
    foreach(QString declaration, splitDeclarations) {
        if(declaration.contains(':')) {
            declaration = declaration.trimmed();
            int colonPosition = declaration.indexOf(':');

            _declarations.insert(declaration.left(colonPosition),
                                 declaration.right(declaration.length()
                                                 - colonPosition - 1));
        }
    }
}

void RuleSet::addChild(RuleSet ruleSet) {
    _children.append(ruleSet);
}

QString RuleSet::toString(QString parentSelector,
                          int indent) {

    QString cssSelector = QString("%1.%2%3%4%5")
            .arg(parentSelector)
            .arg(_block.isEmpty() ? "" : QString("%1__").arg(_block))
            .arg(_isWrapper ? "wrap__" : "")
            .arg(_element)
            .arg(_modifier.isEmpty() ? "" : QString("--%1").arg(_modifier));

    QString indentString;
    indentString.fill(' ', indent);

    QString result = cssSelector + " {\n";

    QStringList properties = _declarations.keys();
    properties.sort();
    foreach(QString property, properties) {
        result += QString("%1%2: %3;\n")
            .arg(indentString)
            .arg(property)
            .arg(_declarations.value(property));
    }

    result += QString("}\n");

    foreach(RuleSet ruleSet, _children) {
        result += "\n";
        result += ruleSet.toString(cssSelector,
                                   indent);
    }

    return result;
}

RuleSet& RuleSet::operator <<(QString declaration) {
    addDeclarations(declaration);
    return *this;
}

RuleSet& RuleSet::operator <<(RuleSet ruleSet) {
    addChild(ruleSet);
    return *this;
}

} // namespace Css

} // namespace QtWebServer
