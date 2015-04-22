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
#include <QByteArray>
#include <QString>
#include <QFile>
#include <QImage>
#include <QPixmap>

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

    /**
     * Takes the provided data and creates a data URL.
     * When no mime type has been provided, it will try to guess
     * the mimetype from the provided data.
     * @returns the UTF-8 encoded data URL.
     */
    static QByteArray encodeDataUrl(DataUrlContents dataUrlContents);

    /**
     * Decodes the data from the provided data url.
     * @param dataUrl The data url that should be decoded.
     * @returns the decoded data.
     */
    static DataUrlContents decodeDataUrl(QByteArray dataUrl);

    /**
     * Convenience method to encode an image to a data url.
     * @param image The image that shall be encoded.
     * @param format The format, @see QImage::save() for more info.
     * @param quality Compression quality, @see QImage::save() for more info.
     * @returns the encoded image data url.
     */
    static QByteArray dataUrlFromImage(QImage image,
                                       const char* format = 0,
                                       int quality = -1);

    /**
     * Reads an image from the data url.
     * @param dataUrl The data url to read from.
     * @returns the image.
     */
    static QImage imageFromDataUrl(QByteArray dataUrl);

    /**
     * Convenience method to encode a pixmap to a data url.
     * @param image The pixmap that shall be encoded.
     * @param format The format, @see QPixmap::save() for more info.
     * @param quality Compression quality, @see QPixmap::save() for more info.
     * @returns the encoded pixmap data url.
     */
    static QByteArray dataUrlFromPixmap(QPixmap pixmap,
                                        const char* format = 0,
                                        int quality = -1);

    /**
     * Reads a pixmap from the data url.
     * @param dataUrl The data url to read from.
     * @returns the pixmap.
     */
    static QPixmap pixmapFromDataUrl(QByteArray dataUrl);

private:
    DataUrlCodec();
    ~DataUrlCodec();
};

} // namespace Util

} // namespace QtWebServer
