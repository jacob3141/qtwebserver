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

    ConnectionPool::ConnectionPool(QObject* parent)
        : QObject(parent)
    {
        QStringList drivers = QSqlDatabase::drivers();

        m_count = 32;

        m_open = false;
        m_hostName = "localhost";
        m_port = 3306;
        m_driverName = drivers.count() ? drivers.at(0) : "";
        m_connectOptions = "";

        m_databaseName = "";
        m_userName = "";
        m_password = "";
    }

    ConnectionPool::~ConnectionPool()
    {
    }

    ConnectionPool& ConnectionPool::instance()
    {
        static ConnectionPool _instance;
        return _instance;
    }

    void ConnectionPool::resize(int count)
    {
        m_count = count;
    }

    int ConnectionPool::count()
    {
        return m_count.r();
    }

    bool ConnectionPool::open()
    {
        close();
        int c = count();
        for (int i = 0; i < c; i++) {
            QSqlDatabase db = QSqlDatabase::addDatabase(driverName(), QString("sql%1").arg(i));
            db.setHostName(hostName());
            db.setPort(port());
            db.setConnectOptions(connectOptions());
            db.setDatabaseName(databaseName());
            db.setUserName(userName());
            db.setPassword(password());

            if (!db.open()) {
                close();
                return false;
            }
        }
        reset();
        m_open = true;
        return true;
    }

    bool ConnectionPool::open(const QString& user, const QString& password)
    {
        m_userName = user;
        m_password = password;
        return open();
    }

    bool ConnectionPool::isOpen()
    {
        return m_open.r();
    }

    void ConnectionPool::close()
    {
        QStringList connectionNames = QSqlDatabase::connectionNames();
        foreach (QString connectionName, connectionNames) {
            QSqlDatabase::removeDatabase(connectionName);
        }
        m_open = false;
    }

    QString ConnectionPool::hostName() const
    {
        return m_hostName.r();
    }

    int ConnectionPool::port() const
    {
        return m_port.r();
    }

    QString ConnectionPool::driverName() const
    {
        return m_driverName.r();
    }

    QString ConnectionPool::connectOptions() const
    {
        return m_connectOptions.r();
    }

    QString ConnectionPool::databaseName() const
    {
        return m_databaseName.r();
    }

    QString ConnectionPool::userName() const
    {
        return m_userName.r();
    }

    QString ConnectionPool::password() const
    {
        return m_password.r();
    }

    QSqlQuery ConnectionPool::exec(const QString& query)
    {
        QSqlDatabase db = QSqlDatabase::database(nextConnectionName());
        return db.exec(query);
    }

    void ConnectionPool::setHostName(QString hostName)
    {
        m_hostName = hostName;
    }

    void ConnectionPool::setPort(int port)
    {
        m_port = port;
    }

    void ConnectionPool::setDriverName(QString driverName)
    {
        m_driverName = driverName;
    }

    void ConnectionPool::setConnectOptions(QString connectOptions)
    {
        m_connectOptions = connectOptions;
    }

    void ConnectionPool::setDatabaseName(QString databaseName)
    {
        m_databaseName = databaseName;
    }

    void ConnectionPool::setUserName(QString userName)
    {
        m_userName = userName;
    }

    void ConnectionPool::setPassword(QString password)
    {
        m_password = password;
    }

    void ConnectionPool::reset()
    {
        m_nextConnectionNameMutex.lock();
        m_next = 0;
        m_nextConnectionNameMutex.unlock();
    }

    QString ConnectionPool::nextConnectionName()
    {
        m_nextConnectionNameMutex.lock();
        m_next++;
        if (m_next >= count()) {
            m_next = 0;
        }
        QString connectionName = QString("sql%1").arg(m_next);
        m_nextConnectionNameMutex.unlock();
        return connectionName;
    }

} // namespace Sql

} // namespace QtWebServer
