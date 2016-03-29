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

// Own includes
#include "http/httpresource.h"

// Qt includes
#include <QMap>
#include <QMimeDatabase>

namespace QtWebServer {

namespace Util {

/**
 * Default controller for delivering assets such as images or other
 * files. For best performance, the asset path should be contained in
 * a resource file if possible as this avoids expensive disk I/O operations.
 * The mimetype is automatically detected. Assets will be made available
 * under the unique resource identifier "/asset/{id}", e.g. if you have
 * added an asset with an id of "logoimage", you can access it via
 * "/asset/logoimage".
 */
class AssetsResource :
    public Http::Resource {
    Q_OBJECT
public:
    AssetsResource(QObject *parent = 0);
    ~AssetsResource();

    /**
     * Inserts a new asset that will be made available at "/asset/{id}".
     * @param id The id value. Must be unique.
     * @param assetPath The physical path to the asset.
     */
    void insertAsset(QString id, QString assetPath);

    /**
     * Removes an asset.
     * @param id The asset's id.
     */
    void removeAsset(QString id);

protected:
    void deliver(const Http::Request& request,
                 Http::Response& response);
private:
    QMap<QString, QString> _assetsPathMap;
    QMimeDatabase _mimeDatabase;
};

} // namespace Util

} // namespace QtWebServer
