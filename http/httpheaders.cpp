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
#include "httpheaders.h"

namespace QtWebServer {

namespace Http {

HeaderNamePair headerNameMap[HEADER_COUNT] = {
    {CacheControl,          "Cache-Control"},
    {Connection,            "Connection"},
    {ContentLength,         "Content-Length"},
    {ContentMD5,            "Content-MD5"},
    {ContentType,           "Content-Type"},
    {Date,                  "Date"},
    {Pragma,                "Pragma"},
    {TransferEncoding,      "Transfer-Encoding"},
    {Via,                   "Via"},
    {Warning,               "Warning"},

    {Accept,                "Accept"},
    {AcceptCharset,         "Accept-Charset"},
    {AcceptEncoding,        "Accept-Encoding"},
    {AcceptLanguage,        "Accept-Language"},
    {Authorization,         "Authorization"},
    {Cookie,                "Cookie"},
    {Expect,                "Expect"},
    {From,                  "From"},
    {Host,                  "Host"},
    {IfMatch,               "If-Match"},
    {IfModifiedSince,       "If-Modified-Since"},
    {IfNoneMatch,           "If-None-Match"},
    {IfRange,               "If-Range"},
    {IfUnmodifiedSince,     "If-Unmodified-Since"},
    {MaxForwards,           "Max-Forwards"},
    {ProxyAuthorization,    "Proxy-Authorization"},
    {Range,                 "Range"},
    {Referer,               "Referer"},
    {TE,                    "TE"},
    {Upgrade,               "Upgrade"},
    {UserAgent,             "User-Agent"},

    {AcceptRanges,          "Accept-Ranges"},
    {Age,                   "Age"},
    {Allow,                 "Allow"},
    {ContentEncoding,       "Content-Encoding"},
    {ContentLanguage,       "Content-Language"},
    {ContentLocation,       "Content-Location"},
    {ContentDisposition,    "Content-Disposition"},
    {ContentRange,          "Content-Range"},
    {ContentSecurityPolicy, "Content-Security-Policy"},
    {ETag,                  "ETag"},
    {Expires,               "Expires"},
    {LastModified,          "Last-Modified"},
    {Link,                  "Link"},
    {Location,              "Location"},
    {P3P,                   "P3P"},
    {ProxyAuthenticate,     "Proxy-Authenticate"},
    {Refresh,               "Refresh"},
    {RetryAfter,            "Retry-After"},
    {Server,                "Server"},
    {SetCookie,             "Set-Cookie"},
    {Trailer,               "Trailer"},
    {Vary,                  "Vary"},
    {WWWAuthenticate,       "WWW-Authenticate"}
};

QString headerName(Http::Header header) {
    for(int i = 0; i < HEADER_COUNT; i++) {
        if(headerNameMap[i].header == header) {
            return headerNameMap[i].headerName;
        }
    }
    return "";
}

} // namespace Http

} // namespace QtWebServer
