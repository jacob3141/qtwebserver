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

// Qt includes
#include <QMutex>

namespace QtWebServer {

class MutexLocker {
public:
    MutexLocker(QMutex& m) : _m(m) { _m.lock(); }
    ~MutexLocker() { _m.unlock(); }

private:
    QMutex& _m;
};

template<typename T>
class ThreadGuard {
public:
    ThreadGuard() {
        _mutex = new QMutex();
    }

    ~ThreadGuard() {
        delete _mutex;
    }

    void set(const T& other) {
        MutexLocker m(*_mutex); Q_UNUSED(m);
        _r = other;
    }

    T r() const {
        MutexLocker m(*_mutex); Q_UNUSED(m);
        return _r;
    }

    const ThreadGuard<T>& operator=(const T& other) {
        set(other);
        return *this;
    }

private:
    ThreadGuard(const ThreadGuard&) {}

    T _r;

    // There is a reason we are holding a pointer instead of a reference:
    // Sometimes const getter methods would lock the mutex to retrieve the
    // guarded value. Strictly speaking, this would also alter the object,
    // so retrieving a value would also mean getters would lose their constness.
    // While correct, this behaviour doesn't make sense, so we keep a pointer
    // here as a workaround.
    QMutex *_mutex;
};

} // namespace QtWebServer
