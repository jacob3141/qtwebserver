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

// Own includes
#include "partial.h"

// Qt includes
#include <QRegExp>
#include <QStringList>

namespace QtWebServer {

namespace Template {

Partial::Partial(QString resource) :
    Logger("Partial") {
    _resource = resource;
}

Partial::~Partial() {
}

QString Partial::resource() {
    return _resource;
}

void Partial::setResource(QString resource) {
    _resource = resource;
}

QString Partial::resolve() {
    QString result;
    if(!_resource.isEmpty()) {
        QFile file(_resource);
        file.open(QFile::ReadOnly);
        if(file.isOpen()) {
            result = QString::fromUtf8(file.readAll());
            file.close();

            QRegExp regExp("\\{\\{([^\\{\\}]+)\\}\\}");
            regExp.indexIn(result);
            QStringList capturedTexts = regExp.capturedTexts();

            capturedTexts.removeDuplicates();
            foreach(QString capturedText, capturedTexts) {
                if(!capturedText.isEmpty()) {
                    Partial partial(capturedText);
                    result.replace("{{" + capturedText + "}}", partial.resolve());
                }
            }
        }
    }

    return result;
}

} // namespace Template

} // namespace QtWebServer

