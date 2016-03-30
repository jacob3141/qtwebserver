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
#include <QString>
#include <QStringList>

// QtWebServer includes
#include "http/httprequest.h"
#include "html/htmldocument.h"

// Own includes
#include "webwidget.h"

namespace QtWebServer {
class WebLayout {
public:
    WebLayout();

    void addCss(QString resourcePath);
    void addJs(QString resourcePath);
    void addWebWidget(WebWidget* webWidget);

    virtual QString renderHtml(const Http::Request& request);

    void setTitle(QString title);
    QString title();

protected:
    QString _title;
    QStringList _cssResources;
    QStringList _jsResources;
    QList<WebWidget*> _webWidgets;
};

} // namespace QtWebServer
