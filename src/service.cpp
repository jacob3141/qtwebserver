#include <QSettings>
#include <QMetaObject>
#include "service.h"
#include <iostream>

Service::Service()
    : QTcpServer() {
    initialize();
    std::cout << "init" << std::flush;
}

Service::~Service() {

}

void Service::initialize() {
    QSettings settings("shark.settings", QSettings::IniFormat);
    settings.beginGroup("service");
    _port = settings.value("port", 80).toInt();
    _threads = settings.value("threads", 4).toInt();
    settings.endGroup();

    // Mark all active threads for deletion if already running
    foreach(WebServiceThread* webServiceThread, _webServiceThreads) {
        if(webServiceThread) {
            webServiceThread->deleteLater();
        }
    }
    _webServiceThreads.clear();

    // Create the specified number of threads and store them in a set
    int thread = _threads;
    while(thread > 0) {
        WebServiceThread* webServiceThread = new WebServiceThread();
        webServiceThread->start();
        _webServiceThreads.append(webServiceThread);
        thread--;
    }

    _nextRequestDelegatedTo = 0;

    // Listen
    listen(QHostAddress::Any, _port);
}

void Service::incomingConnection(int handle) {
    QMetaObject::invokeMethod(_webServiceThreads[_nextRequestDelegatedTo], "serve", Q_ARG(int, handle));
    _nextRequestDelegatedTo++;
    if(_nextRequestDelegatedTo >= _webServiceThreads.size()) {
        _nextRequestDelegatedTo = 0;
    }
}
