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
#include "httprequest.h"
#include "util/utilformurlcodec.h"

// Qt includes
#include <QRegularExpression>
#include <QStringList>

namespace QtWebServer {

namespace Http {

    Request::Request()
        : Logger("WebServer::Http::Request")
    {
        setDefaults();
    }

    Request::Request(const QByteArray& rawRequest)
        : Logger("WebServer::Http::Request")
    {
        setDefaults();
        deserialize(rawRequest);
    }

    bool Request::isValid() const
    {
        return m_valid;
    }

    Method Request::method() const
    {
        return m_method;
    }

    QString Request::uniqueResourceIdentifier() const
    {
        return m_uniqueResourceIdentifier;
    }

    QString Request::version() const
    {
        return m_version;
    }

    QMap<QString, QByteArray> Request::urlParameters() const
    {
        return m_urlParameters;
    }

    QMap<QString, QString> Request::headers() const
    {
        return m_headers;
    }

    QString Request::header(Header header) const
    {
        return this->header(headerName(header));
    }

    QString Request::header(QString headerName) const
    {
        return m_headers.value(headerName);
    }

    QMap<QString, QString> Request::getParameters() const
    {
        return m_getParameters;
    }

    QMap<QString, QString> Request::postParameters() const
    {
        return m_postParameters;
    }

    QByteArray Request::body() const
    {
        return m_body;
    }

    void Request::appendBodyData(QByteArray bodyData)
    {
        m_body.append(bodyData);
    }

    bool Request::isComplete() const
    {
        if (m_headers.contains(headerName(ContentLength))) {
            bool contentLengthValid = false;
            long long contentLength = m_headers.value(headerName(ContentLength)).toLongLong(&contentLengthValid);
            if (contentLengthValid) {
                return m_body.count() == contentLength;
            }
        }

        // TODO: Chunked transfer mode here.

        // If there is nothing indicating the content length
        // and no information about chunked transfer mode then
        // we have to assume the requesrespondt is complete
        return true;
    }

    QByteArray Request::takeLine(QByteArray& rawRequest)
    {
        // Lines in the HTTP protocol are defined to be separated by '\r\n'
        QByteArray line;
        int rawSize = rawRequest.size();
        int rawPosition;

        // Watch out for the first occurrence of '\r'
        for (rawPosition = 0; rawPosition < rawSize - 1; rawPosition++) {
            if (rawRequest[rawPosition] == '\r' && rawRequest[rawPosition + 1] == '\n') {
                break;
            }
        }

        // If there is just two characters left, this must be EOF
        if (rawPosition != rawSize - 2) {
            // Get the line contents
            line = rawRequest.left(rawPosition);

            // Get the remaining data, skipping '\r\n'
            rawRequest = rawRequest.right(rawSize - line.count() - 2);
        } else {
            // EOF
            rawRequest = "";
            line = rawRequest;
        }

        return line;
    }

    void Request::setDefaults()
    {
        m_headers.clear();
        m_getParameters.clear();
        m_postParameters.clear();
        m_urlParameters.clear();
        m_valid = false;
        m_method = Method::GET;
        m_uniqueResourceIdentifier = "";
        m_version = "";
        m_body = "";
    }

    void Request::deserialize(QByteArray rawRequest)
    {
        // Read ahead the first line in the request
        QByteArray rawRequestLine = takeLine(rawRequest);
        QStringList requestLine = QString::fromUtf8(rawRequestLine)
                                      .split(QRegularExpression("\\s+"));

        QStringList rawRequestLines = QString::fromUtf8(rawRequest).split("\n");

        if (requestLine.count() < 3) {
            // The request line has to contain three strings: The method
            // string, the request uri and the HTTP version. If we were
            // strict, we shouldn't even accept anything larger than four
            // strings, but we're permissive here.
            return;
        }

        m_method = Method::UNKNOW;
        QString methodString = requestLine.at(0);
        if (methodString == "OPTIONS") {
            m_method = Method::OPTIONS;
        } else if (methodString == "GET") {
            m_method = Method::GET;
        } else if (methodString == "HEAD") {
            m_method = Method::HEAD;
        } else if (methodString == "POST") {
            m_method = Method::POST;
        } else if (methodString == "PUT") {
            m_method = Method::PUT;
        } else if (methodString == "DELETE") {
            m_method = Method::DELETE;
        } else if (methodString == "TRACE") {
            m_method = Method::TRACE;
        } else if (methodString == "CONNECT") {
            m_method = Method::CONNECT;
        }

        // add post paremeters
        if (m_method == Method::POST) {
            log("We get POST!");
            QString postLine = rawRequestLines.at(rawRequestLines.count() - 1);
            if (!postLine.isEmpty()) {
                QStringList postData = postLine.split('&', Qt::SkipEmptyParts);
                for (int i = 0; i < postData.count(); i++) {
                    QStringList post = postData.at(i).split('=', Qt::SkipEmptyParts);
                    if (post.count() == 2) {
                        m_postParameters.insert(post.at(0), post.at(1));
                    }
                }
            }
        }

        QStringList splittedURI = requestLine.at(1).split('?', Qt::SkipEmptyParts);
        if (splittedURI.count() > 1) {
            m_urlParameters = Util::FormUrlCodec::decodeFormUrl(splittedURI.at(1).toUtf8());
            // add get parameters
            QStringList getData = splittedURI.at(1).split('&', Qt::SkipEmptyParts);
            for (int i = 0; i < getData.count(); i++) {
                QStringList get = getData.at(i).split('=', Qt::SkipEmptyParts);
                if (get.count() == 2) {
                    m_getParameters.insert(get.at(0), get.at(1));
                }
            }
        }

        m_uniqueResourceIdentifier = splittedURI.at(0);

        m_version = requestLine.at(2);

        QByteArray nextLine;
        while (!(nextLine = takeLine(rawRequest)).isEmpty()) {
            deserializeHeader(nextLine);
        }

        // By definition, all that follows after a \r\n\r\n is the body of the request.
        m_body = QByteArray(rawRequest);
        m_valid = true;
    }

    void Request::deserializeHeader(const QByteArray& rawHeader)
    {
        QString headerLine = QString::fromUtf8(rawHeader);
        int colonPosition;
        int headerLineLength = headerLine.count();
        for (colonPosition = 0; colonPosition < headerLineLength; colonPosition++) {
            if (headerLine.at(colonPosition) == ':') {
                break;
            }
        }

        if (colonPosition == headerLineLength) {
            log(QString("Invalid header line found %1").arg(headerLine), Log::Warning);
            return;
        }

        QString headerName = headerLine.left(colonPosition);
        QString headerValue = headerLine.right(headerLineLength - colonPosition - 1).trimmed();
        m_headers.insert(headerName, headerValue);
    }

} // namespace Http

} // namespace QtWebServer
