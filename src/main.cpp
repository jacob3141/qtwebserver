#include <QCoreApplication>
#include "service.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Service *service = new Service();
    service->initialize();
    return a.exec();
}

