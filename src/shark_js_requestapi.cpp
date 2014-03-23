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

// Own includes
#include "shark_js_requestapi.h"
#include "shark_engine.h"

namespace Shark {

namespace Js {

RequestAPI::RequestAPI(Shark::Engine& engine, Http::Request &request, QObject *parent)
    : QObject(parent),
      Logger("Shark::Js::RequestAPI"),
      _request(request),
      _engine(engine) {
}

QStringList RequestAPI::parameters() {
    return _request.availableQueryParameters();
}

QString RequestAPI::parameter(QString name) {
    if(hasParameter(name)) {
        return _request.queryParameter(name);
    } else {
        return "";
    }
}

bool RequestAPI::hasParameter(QString name) {
    return _request.availableQueryParameters().contains(name);
}

QString RequestAPI::requestString() {
    return _request.requestString();
}

} // namespace Js

} // namespace Shark
