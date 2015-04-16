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
#include "utildataurlcodec.h"

// Qt includes
#include <QMimeDatabase>
#include <QMimeType>
#include <QImage>

namespace QtWebServer {

namespace Util {

DataUrlCodec::DataUrlCodec() {
}

DataUrlCodec::~DataUrlCodec() {
}

QByteArray DataUrlCodec::toDataUrl(QByteArray data,
                                   QString mimeType,
                                   bool encodeBase64) {
    QMimeDatabase mimeDatabase;
    QMimeType autoMimeType = mimeDatabase.mimeTypeForData(data);

    QString encodingParamter = "";
    if(encodeBase64) {
        encodingParamter = ";base64";
    }

    QByteArray encodedData;
    if(encodeBase64) {
        encodedData = data.toBase64();
    } else {
        encodedData = data.toPercentEncoding();
    }

    return QString("data:%1;charset=utf-8%2,%3")
        .arg(mimeType.isEmpty() ? autoMimeType.name() : mimeType)
        .arg(encodingParamter)
        .arg(QString::fromUtf8(encodedData))
        .toUtf8();
}

QImage DataUrlCodec::imageFromDataUrl(QByteArray dataUrl) {
    if(!dataUrl.startsWith("data:")) {
        return QImage();
    }

    dataUrl = dataUrl.right(dataUrl.count() - 5);

    QList<QByteArray> dataUrlParts = dataUrl.split(',');

    // data:image/png;charset=utf-8;base64,<data>
    if(dataUrlParts.count() < 2) {
        return QImage();
    }

    // Split into data info and data
    QByteArray dataInfo = dataUrlParts[0];
    QByteArray data = dataUrlParts[1];

    bool base64Encoded = false;
    bool utf8Charset = false;
    QString mimeTypeName = "text/plain";

    QList<QByteArray> dataInfoParts = dataInfo.split(';');
    foreach(QByteArray dataInfoPart, dataInfoParts) {
        if(dataInfoPart == "base64") {
            base64Encoded = true;
        } else
        if(dataInfoPart.startsWith("charset=")) {
            dataInfoPart = dataInfoPart.right(dataInfoPart.count() - 8);
            if(dataInfoPart.toLower() == "utf-8") {
                utf8Charset = true;
            }
        } else {
            mimeTypeName = QString::fromUtf8(dataInfoPart);
        }
    }

    if(base64Encoded) {
        data = QByteArray::fromBase64(data);
    } else {
        data = QByteArray::fromPercentEncoding(data);
    }

    if(!mimeTypeName.startsWith("image/")) {
        return QImage();
    }

    return QImage::fromData(data);
}

} // Util

} // QtWebServer
