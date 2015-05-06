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
#include "httpresource.h"

// Qt includes
#include <QStringList>

namespace QtWebServer {

namespace Http {

Resource::Resource(QString uniqueIdentifier,
                   QObject *parent) :
    QObject(parent) {
    _uniqueIdentifier = uniqueIdentifier;
}

bool Resource::match(QString uniqueIdentifier) {
    // Split both the unique identifier of this resource the one in question,
    // so we can compare those.
    QStringList splittedReferenceUri = this->uniqueIdentifier().split("/", QString::SkipEmptyParts);
    QStringList splittedRequestedUri = uniqueIdentifier.split("/", QString::SkipEmptyParts);

    int count = splittedRequestedUri.count();

    // In case we have a different depth, the unique identifiers cannot match.
    if(splittedReferenceUri.count() != count) {
        return false;
    }

    // Compare each part of the passed unique identifier.
    for(int depth = 0; depth < count; depth++) {
        // In case we have a variable, we do not need an exact match.
        if(splittedReferenceUri.at(depth).startsWith("{") && splittedReferenceUri.at(depth).endsWith("}")) {
            continue;
        }

        // Otherwise, we expect the unique idenfier parts to match exactly.
        if(splittedReferenceUri.at(depth) != splittedRequestedUri.at(depth)) {
            return false;
        }
    }

    return true;
}

QMap<QString, QString> Resource::uriParameters(QString uniqueIdentifier) {
    // Split both the unique identifier of this resource the one in question,
    // so we can compare those.
    QStringList splittedReferenceUri = this->uniqueIdentifier().split("/", QString::SkipEmptyParts);
    QStringList splittedRequestedUri = uniqueIdentifier.split("/", QString::SkipEmptyParts);

    // Create a parameter map.
    QMap<QString, QString> uriParameterMap;

    int count = splittedRequestedUri.count();

    // In case we have a different depth, the unique identifiers cannot match.
    if(splittedReferenceUri.count() != count) {
        return uriParameterMap;
    }

    // Compare each part of the passed unique identifier.
    for(int depth = 0; depth < count; depth++) {
        QString uriSegment = splittedReferenceUri.at(depth);
        if(uriSegment.startsWith("{") && uriSegment.endsWith("}")) {
            // Truncate the { and }
            uriSegment = uriSegment.mid(1, uriSegment.count() - 2);
            QString key = uriSegment;
            QString value = splittedRequestedUri.at(depth);
            uriParameterMap.insert(key, value);
        }
    }

    return uriParameterMap;
}


QString Resource::uniqueIdentifier() {
    return _uniqueIdentifier.r();
}

void Resource::setUniqueIdentifier(QString uniqueIdentifer) {
    _uniqueIdentifier = uniqueIdentifer;
}

QString Resource::contentType() {
    return _contentType.r();
}

void Resource::setContentType(QString contentType) {
    _contentType = contentType;
}

} // namespace Http

} // namespace QtWebServer
