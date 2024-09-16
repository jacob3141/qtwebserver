#include <QCoreApplication>
#include <QFile>

#include "http/bytearrayresource.h"
#include "http/httpwebengine.h"
#include "tcp/tcpmultithreadedserver.h"

using namespace QtWebServer;

int main(int argc, char* argv[])
{
    QCoreApplication a(argc, argv);

    Tcp::MultithreadedServer s;
    Http::WebEngine w;

    w.addResource(new Http::ByteArrayResource(
        "/",
        QByteArray("Hello world")));

    s.setResponder(&w);
    s.listen(QHostAddress::Any, 3000);
    return a.exec();
}
