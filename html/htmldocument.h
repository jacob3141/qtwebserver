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

#pragma once

// Qt includes
#include <QDomDocument>
#include <QList>

namespace QtWebServer {

namespace Html {

/**
 * Html document. This extends Qt's default QDomDocument by some
 * useful methods.
 */
class Document :
    public QDomDocument {
public:
    Document(QString documentTypeDeclaration = "<!DOCTYPE html>");
    ~Document();

    /** Sets the title element's value. */
    void setTitle(QString title);

    /** @returns the title element's value. */
    QString title();

    /** @returns the html DOM element. */
    QDomElement html();
    /** @returns the head DOM element. */
    QDomElement head();
    /** @returns the body DOM element. */
    QDomElement body();

    /**
     * Appends a partial to the specific element in the DOM tree.
     * @param domElement The element that this partial will be added to.
     * @param resourceName The physical resource name.
     * @returns true, if the partial could be appended.
     */
    bool appendPartial(QDomElement domElement, QString resourceName);

    /**
     * Parses and append the given HTML to the DOM.
     * @param domElement The DOM element the parsed HTML shall be added to.
     * @param html The html code that shall be added.
     * @returns true, if the html code could be appended.
     */
    bool appendHtml(QDomElement domElement, QString html);

    /**
     * Search the document recursively for the given class. For performance
     * reasons, always try to use the version that operates on a DOM subtree.
     * @param className The class name to search for.
     * @returns a list of matching elements.
     */
    QList<QDomElement> elementsByClass(QString className) const;

    /** Like elementsByClass(), but operations on a DOM subtree. */
    QList<QDomElement> elementsByClass(QDomElement domElement, QString className) const;

    /** @returns the first occurrence of an element with the given id attribute. */
    QDomElement elementById(QString idName) const;

    /**
     * Search the document recursively for the given id. For performance
     * reasons, always try to use the version that operates on a DOM subtree.
     * @param idName The id name to search for.
     * @returns a list of matching elements.
     */
    QList<QDomElement> elementsById(QString idName) const;

    /** Like elementsById(), but operations on a DOM subtree. */
    QList<QDomElement> elementsById(QDomElement domElement, QString idName) const;

    /**
     * Search the document recursively for an attribute. For performance
     * reasons, always try to use the version that operates on a DOM subtree.
     * @param attributeName The attribute's name.
     * @param attributeValue The attribute's value.
     * @paramt allowMultipleValues Indicates the the attribute may contain multiple values.
     */
    QList<QDomElement> elementsByAttribute(QString attributeName,
                                           QString attributeValue,
                                           bool allowMultipleValues = true) const;

    /** Like elementsByAttribute(), but operations on a DOM subtree. */
    QList<QDomElement> elementsByAttribute(QDomElement domElement,
                                           QString attributeName,
                                           QString attributeValue,
                                           bool allowMultipleValues = true) const;
private:
    QDomElement _html, _head, _body;
    QDomText _title;
};

} // namespace Html

} // namespace QtWebServer
