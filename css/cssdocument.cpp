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
#include "cssdocument.h"

namespace QtWebServer {

namespace Css {

Document::Document() {
}

Document::~Document() {
}

void Document::addRuleSet(RuleSet ruleSet) {
    _ruleSets.append(ruleSet);
}

Document& Document::operator <<(RuleSet ruleSet) {
    addRuleSet(ruleSet);
    return *this;
}

QByteArray Document::toByteArray(int indent) {
    QByteArray byteArray;
    foreach(RuleSet ruleSet, _ruleSets) {
        byteArray += ruleSet.toString("", indent).toUtf8() + "\n";
    }
    return byteArray;
}

QString Document::toString(int indent) {
    return QString::fromUtf8(toByteArray(indent));
}

} // namespace Css

} // namespace QtWebServer

