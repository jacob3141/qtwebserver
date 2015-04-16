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

namespace QtWebServer {

namespace Http {

/**
 * @brief The Header enum
 */
enum Header {
    // Request and response headers
    CacheControl,
    Connection,
    ContentLength,
    ContentMD5,
    ContentType,
    Date,
    Pragma,
    TransferEncoding,
    Via,
    Warning,

    // Request headers
    Accept,
    AcceptCharset,
    AcceptEncoding,
    AcceptLanguage,
    Authorization,
    Cookie,
    Expect,
    From,
    Host,
    IfMatch,
    IfModifiedSince,
    IfNoneMatch,
    IfRange,
    IfUnmodifiedSince,
    MaxForwards,
    ProxyAuthorization,
    Range,
    Referer,
    TE,
    Upgrade,
    UserAgent,

    // Response headers
    AcceptRanges,
    Age,
    Allow,
    ContentEncoding,
    ContentLanguage,
    ContentLocation,
    ContentDisposition,
    ContentRange,
    ContentSecurityPolicy,
    ETag,
    Expires,
    LastModified,
    Link,
    Location,
    P3P,
    ProxyAuthenticate,
    Refresh,
    RetryAfter,
    Server,
    SetCookie,
    Trailer,
    Vary,
    WWWAuthenticate
};

typedef struct {
    Header header;
    const char* headerName;
} HeaderNamePair;

#define HEADER_COUNT 54

extern HeaderNamePair headerNameMap[HEADER_COUNT];

QString headerName(Http::Header header);

} // namespace Http

} // namespace QtWebServer

