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

#ifndef SHARK_JS_DOMELEMENT_H
#define SHARK_JS_DOMELEMENT_H

// Own includes
#include "shark_engine.h"

// Qt includes
#include <QObject>
#include <QDomElement>

namespace Shark {

namespace Js {

class DomElement : public QObject {
    Q_OBJECT
public:
    DomElement(Shark::Engine& engine, QDomElement domElement, QObject *parent = 0);

private:
    Shark::Engine& _engine;
    QDomElement _domElement;
};

} // namespace Js

} // namespace Shark

#endif // SHARK_JS_DOMELEMENT_H
