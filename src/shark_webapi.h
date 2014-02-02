//
// Copyright 2010-2014 Jacob Dawid <jacob.dawid@cybercatalyst.net>
//
// This file is part of Shark.
//
// Shark is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// Shark is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with Shark.  If not, see <http://www.gnu.org/licenses/>.
//

#ifndef SHARK_WEBAPI_H
#define SHARK_WEBAPI_H

// Qt includes
#include <QObject>
#include <QStack>
#include <QString>

namespace Shark {

/**
 * The WebAPI class represents an object in the global script
 * environment. QtScript exposes its slots as functions into the
 * script space, where it can be accessed through the global "$$"
 * property. Thus, Shark WebAPI is just an extension to JS.
 */
class WebAPI : public QObject {
    Q_OBJECT
public:
    WebAPI(QObject *parent = 0);

public slots:
    bool isEmpty();
    QString clear();
    void push(QString contents);
    QString pop();

    QString time();

private:
    QStack<QString> _stack;
};

} // namespace Shark

#endif // SHARK_WEBAPI_H
