#include "bytearrayresource.h"
namespace QtWebServer {

namespace Http {

    ByteArrayResource::ByteArrayResource(QString uniqueIdentifier, QByteArray data, QObject* parent)
        : Resource(uniqueIdentifier, parent)
        , m_data(data)
    {
        setContentType("text/plain");
    }

    ByteArrayResource::~ByteArrayResource()
    {
    }

    void ByteArrayResource::deliver(const Request& request, Response& response)
    {
        if (request.method() == Method::GET) {
            response.setHeader(Http::ContentType, contentType());
            response.setBody(m_data);
            response.setStatusCode(StatusCode::Ok);
        }
    }
}
}
