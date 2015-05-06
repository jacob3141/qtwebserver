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

/**
 * @class ConnectionPool
 * @author Jacob Dawid
 * Connection pooling.
 */
class ConnectionPool :
    public QObject {
    Q_OBJECT
public:
    /** Singleton. */
    static ConnectionPool &instance();
    ~ConnectionPool();

    /**
     * Resizes the connection pool.
     * @attention For this to take effect you have to close and reopen the
     * connection pool.
     * @param count The number of simultaneous connections.
     */
    void resize(int count);

    /** @returns the number of available simultaneous connections. */
    int count();

    /**
     * Opens the connection pool.
     * @returns true, when the operation was successful.
     */
    bool open();

    /**
     * Opens the connection pool and saves the username and password.
     * @param user The database username to be used.
     * @param password The password for the database user.
     * @returns true, when the operation was successful.
     */
    bool open(const QString& user, const QString& password);

    /** @returns true, when the pool is currently open. */
    bool isOpen();

    /** Closes the connection pool. */
    void close();

    /** @returns the host name. */
    QString	hostName() const;

    /** @returns the port. */
    int	port() const;

    /** @returns the database driver's name. */
    QString	driverName() const;

    /** @returns the database connect options. */
    QString	connectOptions() const;

    /** @returns the database name. */
    QString	databaseName() const;

    /** @returns the database username. */
    QString	userName() const;

    /** @returns the database user's password. */
    QString	password() const;

    /**
     * Executes the query and returns a query object.
     * @param query The SQL query to be executed.
     * @returns the according QSqlQuery object.
     */
    QSqlQuery exec(const QString& query = QString());

    /** Sets the database host name. */
    void setHostName(QString hostName);

    /** Sets the port. */
    void setPort(int port);

    /** Sets the database driver name. */
    void setDriverName(QString driverName);

    /** Sets the database connect options. */
    void setConnectOptions(QString connectOptions);

    /** Sets the database name. */
    void setDatabaseName(QString databaseName);

    /** Sets the database user name. */
    void setUserName(QString userName);

    /** Sets the database user's password. */
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
