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
#include <QString>
#include <QMap>
#include <QList>

namespace QtWebServer {

namespace Css {

class RuleSet {
public:
    RuleSet();
    RuleSet(QString block = "",
            QString element = "*",
            QString modifier = "",
            bool isWrapper = false);
    ~RuleSet();

    QString block();
    void setBlock(QString block);

    QString element();
    void setElement(QString element);

    QString modifier();
    void setModifier(QString modifier);

    bool isWrapper();
    void setWrapper(bool isWrapper);

    void addDeclaration(QString property, QString value);
    void addDeclarations(QString declarations);
    void addChild(RuleSet ruleSet);

    QString toString(QString parentSelector = "",
                     int indent = 4);

    RuleSet& operator <<(QString declaration);
    RuleSet& operator <<(RuleSet ruleSet);

private:
    QString _block;
    QString _element;
    QString _modifier;
    bool _isWrapper;

    QMap<QString, QString> _declarations;

    QList<RuleSet> _children;
};

} // namespace Css

} // namespace QtWebServer
