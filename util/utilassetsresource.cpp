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
#include "utilassetsresource.h"

// Qt includes
#include <QFile>
#include <QFileInfo>

namespace QtWebServer {

namespace Util {

AssetsResource::AssetsResource(QObject *parent) :
    Http::Resource("/asset/{id}", parent) {
}

AssetsResource::~AssetsResource() {
}

void AssetsResource::insertAsset(QString id, QString assetPath) {
    _assetsPathMap.insert(id, assetPath);
}

void AssetsResource::removeAsset(QString id) {
    _assetsPathMap.remove(id);
}

void AssetsResource::respond(const Http::Request& request,
                    Http::Response& response) {
    QString id = uriParameters(request.uniqueResourceIdentifier()).value("id");

    if(_assetsPathMap.contains(id)) {
        QFile assetFile(_assetsPathMap.value(id));
        assetFile.open(QFile::ReadOnly);
        if(assetFile.isOpen()) {
            QFileInfo fileInfo(assetFile);
            response.setStatusCode(Http::Ok);
            response.setHeader(Http::ContentType, _mimeDatabase.mimeTypeForFile(fileInfo).name());
            response.setBody(assetFile.readAll());
            assetFile.close();
        } else {
            response.setStatusCode(Http::Forbidden);
            response.setHeader(Http::ContentType, "text/plain");
        }
    } else {
        response.setStatusCode(Http::NotFound);
        response.setHeader(Http::ContentType, "text/plain");
    }
}

} // namespace Util

} // namespace QtWebServer
