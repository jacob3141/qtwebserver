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

    RuleSet::RuleSet()
        : m_element("*")
    {
    }

    RuleSet::RuleSet(QString block,
        QString element,
        QString modifier,
        bool isWrapper)
        : m_block(block)
        , m_element(element)
        , m_modifier(modifier)
        , m_isWrapper(isWrapper)
    {
    }

    RuleSet::~RuleSet()
    {
    }

    QString RuleSet::block()
    {
        return m_block;
    }

    void RuleSet::setBlock(QString block)
    {
        m_block = block;
    }

    QString RuleSet::element()
    {
        return m_element;
    }

    void RuleSet::setElement(QString selector)
    {
        m_element = selector.trimmed();
    }

    QString RuleSet::modifier()
    {
        return m_modifier;
    }

    void RuleSet::setModifier(QString modifier)
    {
        m_modifier = modifier;
    }

    bool RuleSet::isWrapper()
    {
        return m_isWrapper;
    }

    void RuleSet::setWrapper(bool isWrapper)
    {
        m_isWrapper = isWrapper;
    }

    void RuleSet::addDeclaration(QString property,
        QString value)
    {
        m_declarations.insert(property, value);
    }

    void RuleSet::addDeclarations(QString declarations)
    {
        QStringList splitDeclarations = declarations.split(";", Qt::SkipEmptyParts);
        foreach (QString declaration, splitDeclarations) {
            if (declaration.contains(':')) {
                declaration = declaration.trimmed();
                int colonPosition = declaration.indexOf(':');

                m_declarations.insert(declaration.left(colonPosition),
                    declaration.right(declaration.length()
                        - colonPosition - 1));
            }
        }
    }

    void RuleSet::addChild(RuleSet ruleSet)
    {
        m_children.append(ruleSet);
    }

    QString RuleSet::toString(QString parentSelector,
        int indent)
    {

        QString cssSelector = QString("%1.%2%3%4%5")
                                  .arg(parentSelector)
                                  .arg(m_block.isEmpty() ? "" : QString("%1__").arg(m_block))
                                  .arg(m_isWrapper ? "wrap__" : "")
                                  .arg(m_element)
                                  .arg(m_modifier.isEmpty() ? "" : QString("--%1").arg(m_modifier));

        QString indentString;
        indentString.fill(' ', indent);

        QString result = cssSelector + " {\n";

        QStringList properties = m_declarations.keys();
        properties.sort();
        foreach (QString property, properties) {
            result += QString("%1%2: %3;\n")
                          .arg(indentString)
                          .arg(property)
                          .arg(m_declarations.value(property));
        }

        result += QString("}\n");

        foreach (RuleSet ruleSet, m_children) {
            result += "\n";
            result += ruleSet.toString(cssSelector,
                indent);
        }

        return result;
    }

    RuleSet& RuleSet::operator<<(QString declaration)
    {
        addDeclarations(declaration);
        return *this;
    }

    RuleSet& RuleSet::operator<<(RuleSet ruleSet)
    {
        addChild(ruleSet);
        return *this;
    }

} // namespace Css

} // namespace QtWebServer
