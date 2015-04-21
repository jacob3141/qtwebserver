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
#include <QBuffer>

namespace QtWebServer {

namespace Util {

DataUrlCodec::DataUrlCodec() {
}

DataUrlCodec::~DataUrlCodec() {
}

QByteArray DataUrlCodec::encodeDataUrl(DataUrlContents dataUrlContents) {
    if(dataUrlContents.mimeTypeName.isEmpty()) {
        QMimeDatabase mimeDatabase;
        QMimeType mimeType = mimeDatabase.mimeTypeForData(dataUrlContents.data);
        dataUrlContents.mimeTypeName = mimeType.name();
    }

    QString encodingParamter;
    QByteArray encodedData;
    if(dataUrlContents.base64Encoded) {
        encodingParamter = ";base64";
        encodedData = dataUrlContents.data.toBase64();
    } else {
        encodingParamter = "";
        encodedData = dataUrlContents.data.toPercentEncoding();
    }

    QString charsetString;
    if(dataUrlContents.charset.toLower() == "utf-8") {
        charsetString = ";charset=utf-8";
    } else {
        charsetString = "";
    }

    QString dataUrlString = QString("data:%1%2%3,%4")
            .arg(dataUrlContents.mimeTypeName)
            .arg(charsetString)
            .arg(encodingParamter)
            .arg(QString::fromUtf8(encodedData));

    if(dataUrlContents.charset.toLower() == "utf-8") {
        return dataUrlString.toUtf8();
    } else {
        return dataUrlString.toLatin1();
    }
}

DataUrlCodec::DataUrlContents DataUrlCodec::decodeDataUrl(QByteArray dataUrl) {
    DataUrlContents dataUrlContents;

    if(!dataUrl.startsWith("data:")) {
        return dataUrlContents;
    }

    // Remove the data: prefix by removing the first five characters
    dataUrl = dataUrl.right(dataUrl.count() - 5);

    // Split at comma to separate the preamble from the actual data
    // data:image/png;charset=utf-8;base64,<data>
    QList<QByteArray> dataUrlParts = dataUrl.split(',');

    // If there is no separation between preamle and data, the url is
    // invalid
    if(dataUrlParts.count() < 2) {
        return dataUrlContents;
    }

    // Split into data info and data
    QByteArray dataInfo = dataUrlParts[0];
    dataUrlContents.data = dataUrlParts[1];

    // Interpret the single preamble parts
    QList<QByteArray> dataInfoParts = dataInfo.split(';');
    foreach(QByteArray dataInfoPart, dataInfoParts) {
        if(dataInfoPart == "base64") {
            dataUrlContents.base64Encoded = true;
        } else
        if(dataInfoPart.startsWith("charset=")) {
            dataInfoPart = dataInfoPart.right(dataInfoPart.count() - 8);
            dataUrlContents.charset = QString::fromUtf8(dataInfoPart.toLower());
        } else {
            dataUrlContents.mimeTypeName = QString::fromUtf8(dataInfoPart);
        }
    }

    // Decode data depending on whether it has been base64 or percent encoded
    if(dataUrlContents.base64Encoded) {
        dataUrlContents.data = QByteArray::fromBase64(dataUrlContents.data);
    } else {
        dataUrlContents.data = QByteArray::fromPercentEncoding(dataUrlContents.data);
    }

    return dataUrlContents;
}

QByteArray DataUrlCodec::dataUrlFromImage(QImage image,
                                          const char *format,
                                          int quality) {
    DataUrlContents dataUrlContents;
    QBuffer buffer(&dataUrlContents.data);
    buffer.open(QIODevice::WriteOnly);
    if(buffer.isOpen()) {
        image.save(&buffer, format, quality);
        buffer.close();
    }
    return encodeDataUrl(dataUrlContents);
}

QImage DataUrlCodec::imageFromDataUrl(QByteArray dataUrl) {
    DataUrlContents dataUrlContents = decodeDataUrl(dataUrl);

    if(!dataUrlContents.mimeTypeName.startsWith("image/")) {
        return QImage();
    }

    return QImage::fromData(dataUrlContents.data);
}

} // Util

} // QtWebServer
