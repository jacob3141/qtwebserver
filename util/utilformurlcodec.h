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
#include <QMap>
#include <QString>
#include <QByteArray>

namespace QtWebServer {

namespace Util {

class FormUrlCodec {
public:
    /** Encodes the form data into an URL. */
    static QByteArray encodeFormUrl(QMap<QString, QByteArray> formData);

    /** Decodes the form data from an URL. */
    static QMap<QString, QByteArray> decodeFormUrl(QByteArray formUrl);

private:
    FormUrlCodec();
    ~FormUrlCodec();
};

} // namespace Util

} // namespace QtWebServer
