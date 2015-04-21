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

#include <QByteArray>
#include <QString>
#include <QFile>

namespace QtWebServer {

namespace Util {

class DataUrlCodec {
public:
    struct DataUrlContents {
        DataUrlContents() {
            data = "";
            mimeTypeName = "";
            charset = "";
            base64Encoded = false;
        }

        QByteArray data;
        QString mimeTypeName;
        QString charset;
        bool base64Encoded;
    };

    DataUrlCodec();
    ~DataUrlCodec();

    /**
     * Takes the provided data and creates a data URL.
     * When no mime type has been provided, it will try to guess
     * the mimetype from the provided data.
     * @param data The binary data to be encoded.
     * @param mimeType Explicit mimetype.
     * @param encodeBase64 Encode as base64 string, percent encoding otherwise.
     * @returns the UTF-8 encoded data URL.
     */
    static QByteArray encodeDataUrl(DataUrlContents dataUrlContents);

    static DataUrlContents decodeDataUrl(QByteArray dataUrl);

    static QByteArray dataUrlFromImage(QImage image,
                                       const char* format = 0,
                                       int quality = -1);

    static QImage imageFromDataUrl(QByteArray dataUrl);

};

} // namespace Util

} // namespace QtWebServer
