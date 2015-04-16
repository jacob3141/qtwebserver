//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public
// License along with QtWebServer.
// If not, see <http://www.gnu.org/licenses/>.
//
// It is possible to obtain a commercial license of QtWebServer.
// Please contact Jacob Dawid <jacob@omg-it.works>
//

// Own includes
#include "sqlconnectionpool.h"

// Qt includes
#include <QStringList>

namespace QtWebServer {

namespace Sql {

ConnectionPool::ConnectionPool(QObject *parent) :
    QObject(parent) {
    QStringList drivers = QSqlDatabase::drivers();

    _count = 32;

    _open = false;
    _hostName = "localhost";
    _port = 3306;
    _driverName = drivers.count() ? drivers.at(0) : "";
    _connectOptions = "";

    _databaseName = "";
    _userName = "";
    _password = "";
}

ConnectionPool::~ConnectionPool() {
}

ConnectionPool& ConnectionPool::instance() {
    static ConnectionPool _instance;
    return _instance;
}

void ConnectionPool::resize(int count) {
    _count = count;
}

int ConnectionPool::count() {
    return _count.r();
}

bool ConnectionPool::open() {
    close();
    int c = count();
    for(int i = 0; i < c; i++) {
        QSqlDatabase db = QSqlDatabase::addDatabase(driverName(), QString("sql%1").arg(i));
        db.setHostName(hostName());
        db.setPort(port());
        db.setConnectOptions(connectOptions());
        db.setDatabaseName(databaseName());
        db.setUserName(userName());
        db.setPassword(password());

        if(!db.open()) {
            close();
            return false;
        }
    }
    reset();
    _open = true;
    return true;
}

bool ConnectionPool::open(const QString& user, const QString& password) {
    _userName = user;
    _password = password;
    return open();
}

bool ConnectionPool::isOpen() {
    return _open.r();
}

void ConnectionPool::close() {
    QStringList connectionNames = QSqlDatabase::connectionNames();
    foreach(QString connectionName, connectionNames) {
        QSqlDatabase::removeDatabase(connectionName);
    }
    _open = false;
}

QString	ConnectionPool::hostName() const {
    return _hostName.r();
}

int	ConnectionPool::port() const {
    return _port.r();
}

QString	ConnectionPool::driverName() const {
    return _driverName.r();
}

QString	ConnectionPool::connectOptions() const {
    return _connectOptions.r();
}

QString	ConnectionPool::databaseName() const {
    return _databaseName.r();
}

QString	ConnectionPool::userName() const {
    return _userName.r();
}

QString	ConnectionPool::password() const {
    return _password.r();
}

QSqlQuery ConnectionPool::exec(const QString& query) {
    QSqlDatabase db = QSqlDatabase::database(nextConnectionName());
    return db.exec(query);
}

void ConnectionPool::setHostName(QString hostName) {
    _hostName = hostName;
}

void ConnectionPool::setPort(int port) {
    _port = port;
}

void ConnectionPool::setDriverName(QString driverName) {
    _driverName = driverName;
}

void ConnectionPool::setConnectOptions(QString connectOptions) {
    _connectOptions = connectOptions;
}

void ConnectionPool::setDatabaseName(QString databaseName) {
    _databaseName = databaseName;
}

void ConnectionPool::setUserName(QString userName) {
    _userName = userName;
}

void ConnectionPool::setPassword(QString password) {
    _password = password;
}

void ConnectionPool::reset() {
    _nextConnectionNameMutex.lock();
    _next = 0;
    _nextConnectionNameMutex.unlock();
}

QString ConnectionPool::nextConnectionName() {
    _nextConnectionNameMutex.lock();
    _next++;
    if(_next >= count()) {
        _next = 0;
    }
    QString connectionName = QString("sql%1").arg(_next);
    _nextConnectionNameMutex.unlock();
    return connectionName;
}

} // namespace Sql

} // namespace QtWebServer
