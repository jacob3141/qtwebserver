//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
//
// This file is part of QtWebServer.
//
// QtWebServer is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// QtWebServer is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with QtWebServer.  If not, see <http://www.gnu.org/licenses/>.
//

#pragma once

// Own includes
#include "misc/threadsafety.h"

// Qt includes
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QList>

namespace QtWebServer {

namespace Sql {

class ConnectionPool :
    public QObject {
    Q_OBJECT
public:
    static ConnectionPool &instance();
    ~ConnectionPool();

    void resize(int count);
    int count();

    bool open();
    bool open(const QString& user, const QString& password);
    bool isOpen();
    void close();

    QString	hostName() const;
    int	port() const;
    QString	driverName() const;
    QString	connectOptions() const;

    QString	databaseName() const;
    QString	userName() const;
    QString	password() const;

    QSqlQuery exec(const QString& query = QString());

    void setHostName(QString hostName);
    void setPort(int port);
    void setDriverName(QString driverName);
    void setConnectOptions(QString connectOptions);

    void setDatabaseName(QString databaseName);
    void setUserName(QString userName);
    void setPassword(QString password);

private:
    explicit ConnectionPool(QObject *parent = 0);

    void reset();
    QString nextConnectionName();

    ThreadGuard<bool> _open;

    ThreadGuard<QString> _hostName;
    ThreadGuard<int> _port;
    ThreadGuard<QString> _driverName;
    ThreadGuard<QString> _connectOptions;

    ThreadGuard<QString> _databaseName;
    ThreadGuard<QString> _userName;
    ThreadGuard<QString> _password;

    ThreadGuard<int> _count;
    QMutex _nextConnectionNameMutex;
    int _next;
};

} // namespace Sql

} // namespace QtWebServer
