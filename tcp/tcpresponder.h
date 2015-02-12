#pragma once

#include <QByteArray>

namespace QtWebServer {

namespace Tcp {

class Responder {
public:
    /**
     * @brief respond
     * @param request
     * @param response
     */
    virtual void respond(const QByteArray& request, QByteArray& response) = 0;
};

} // namespace Tcp

} // namespace QtWebServer

