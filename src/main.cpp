#include <QCoreApplication>
#include "service.h"
#include <iostream>

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);
    Service service();
    Q_UNUSED(service);
    return a.exec();
}

