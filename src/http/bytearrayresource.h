#pragma once

// Own includes
#include "httpresource.h"

// Qt includes
#include <QIODevice>

namespace QtWebServer {

namespace Http {

    class ByteArrayResource : public Resource {
        Q_OBJECT

    public:
        ByteArrayResource(QString uniqueIdentifier,
            QByteArray data,
            QObject* parent = nullptr);

        ~ByteArrayResource();

        virtual void deliver(const Request& request, Response& response);

    private:
        QByteArray m_data;
    };
}
}
