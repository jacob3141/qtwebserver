#ifndef WEBSERVICETHREAD_H
#define WEBSERVICETHREAD_H

#include <QThread>
#include <QMutex>

class WebServiceThread : public QThread {
    Q_OBJECT
public:
    WebServiceThread(QObject *parent = 0);
    virtual ~WebServiceThread();

    enum WebServiceState {
        Idle,
        ProcessingRequest,
        ProcessingResponse
    };

    WebServiceState webServiceState();
public slots:
    void serve(int socketHandle);

private slots:
    void readClient();
    void discardClient();

private:
    void setWebServiceState(WebServiceState state);

    QMutex          _webServiceStateMutex;
    WebServiceState _webServiceState;
};

#endif // WEBSERVICETHREAD_H
