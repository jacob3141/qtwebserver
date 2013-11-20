#ifndef SERVICE_H
#define SERVICE_H

#include <QTcpServer>
#include <QVector>

#include "webservicethread.h"

class Service : public QTcpServer {
    Q_OBJECT
public:
    Service();
    virtual ~Service();

    void initialize();


protected:
    void incomingConnection(qintptr handle);

private:

    int _port;
    int _threads;

    int _nextRequestDelegatedTo;
    QVector<WebServiceThread*> _webServiceThreads;
};

#endif // SERVICE_H
