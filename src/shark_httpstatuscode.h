//
// Copyright 2010-2014 Jacob Dawid <jacob.dawid@cybercatalyst.net>
//
// This file is part of Shark.
//
// Shark is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shark is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shark.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SHARK_HTTPSTATUSCODES_H
#define SHARK_HTTPSTATUSCODES_H

// Qt includes
#include <QMap>
#include <QString>

namespace Shark {

namespace Http {

/**
 * @brief The StatusCode enum
 */
enum StatusCode {
    Continue                    = 100,
    SwitchingProtocols          = 101,

    Ok                          = 200,
    Created                     = 201,
    Accepted                    = 202,
    NonAuthoritativeInformation = 203,
    NoContent                   = 204,
    ResetContent                = 205,
    PartialContent              = 206,

    MultipleChoices             = 300,
    MovedPermanently            = 301,
    Found                       = 302,
    SeeOther                    = 303,
    NotModified                 = 304,
    UseProxy                    = 305,
    Unused                      = 306,
    TemporaryRedirect           = 307,

    BadRequest                  = 400,
    Unauthorized                = 401,
    PaymentRequired             = 402,
    Forbidden                   = 403,
    NotFound                    = 404,
    MethodNotAllowed            = 405,
    NotAcceptable               = 406,
    ProxyAuthenticationRequired = 407,
    RequestTimeout              = 408,
    Conflict                    = 409,
    Gone                        = 410,
    LengthRequired              = 411,
    PreconditionFailed          = 412,
    RequestEntityTooLarge       = 413,
    RequestURITooLong           = 414,
    UnsupportedMediaType        = 415,
    RequestedRangeNotSatisfiable= 416,
    ExpectationFailed           = 417,

    InternalServerError         = 500,
    NotImplemented              = 501,
    BadGateway                  = 502,
    ServiceUnavailable          = 503,
    GatewayTimeout              = 504,
    HTTPVersionNotSupported     = 505
};

/**
 * @brief ReasonPhrasePair
 */
typedef struct {
    StatusCode statusCode;
    const char* reasonPhrase;
} ReasonPhrasePair;

#define STATUS_CODE_COUNT 41

/**
 * @brief reasonPhrasePairMap
 */
extern ReasonPhrasePair reasonPhrasePairMap[STATUS_CODE_COUNT];

/**
 * @brief reasonPhrase
 * @param statusCode
 * @return
 */
QString reasonPhrase(Http::StatusCode statusCode);

#define HTML(x) #x

} // namespace Http

} // namespace Shark

#endif // SHARK_HTTPSTATUSCODES_H
