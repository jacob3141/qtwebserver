#include "webservicethread.h"

#include <QTcpSocket>
#include <QStringList>
#include <QDateTime>

WebServiceThread::WebServiceThread(QObject *parent)
    : QThread(parent) {
    _webServiceState = Idle;
}

WebServiceThread::~WebServiceThread() {
}


WebServiceThread::WebServiceState WebServiceThread::webServiceState() {
    _webServiceStateMutex.lock();
    WebServiceState state = _webServiceState;
    _webServiceStateMutex.unlock();
    return state;
}

void WebServiceThread::setWebServiceState(WebServiceThread::WebServiceState state) {
    _webServiceStateMutex.lock();
    _webServiceState = state;
    _webServiceStateMutex.unlock();
}

void WebServiceThread::serve(int socketHandle) {
    QTcpSocket* tcpSocket = new QTcpSocket(this);
    connect(tcpSocket, SIGNAL(readyRead()), this, SLOT(readClient()));
    connect(tcpSocket, SIGNAL(disconnected()), this, SLOT(discardClient()));
    tcpSocket->setSocketDescriptor(socketHandle);
}

void WebServiceThread::readClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
             if (socket->canReadLine()) {
                 QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
                 if (tokens[0] == "GET") {
                     QTextStream os(socket);
                     os.setAutoDetectUnicode(true);
                     os << "HTTP/1.0 200 Ok\r\n"
                         "Content-Type: text/html; charset=\"utf-8\"\r\n"
                         "\r\n"
                         "<h1>Nothing to see here</h1>\n"
                         << QDateTime::currentDateTime().toString() << "\n";
                     socket->close();

                     if (socket->state() == QTcpSocket::UnconnectedState) {
                         delete socket;
                     }
                 }
             }
}

void WebServiceThread::discardClient() {
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();

}
