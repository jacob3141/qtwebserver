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
#include "utilformurlcodec.h"

// Qt includes
#include <QStringList>

namespace QtWebServer {

namespace Util {

FormUrlCodec::FormUrlCodec() {
}

FormUrlCodec::~FormUrlCodec() {
}

QByteArray FormUrlCodec::encodeFormUrl(QMap<QString, QByteArray> formData) {
    QByteArray formUrl;
    QStringList keys = formData.keys();
    foreach(QString key, keys) {
        if(!formUrl.isEmpty()) {
            formUrl.append('&');
        }
        formUrl.append(QString("%1=%2")
            .arg(key)
            .arg(QString::fromUtf8(formData.value(key).toPercentEncoding())).toUtf8());
    }
    return formUrl;
}

QMap<QString, QByteArray> FormUrlCodec::decodeFormUrl(QByteArray formUrl) {
    QMap<QString, QByteArray> formData;
    QList<QByteArray> formFields = formUrl.split('&');
    foreach(QByteArray formField, formFields) {
        QList<QByteArray> nameValue = formField.split('=');
        if(nameValue.count() >= 2) {
            QString fieldName = QString::fromUtf8(nameValue.at(0));
            QByteArray fieldValue = QByteArray::fromPercentEncoding(nameValue.at(1));
            formData.insert(fieldName, fieldValue);
        }
    }
    return formData;
}

} // namespace Util

} // namespace QtWebServer
