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

#include "weblayout.h"

#include "html/htmldocument.h"

namespace QtWebServer {

WebLayout::WebLayout() {
}

void WebLayout::addCss(QString resourcePath) {
    _cssResources << resourcePath;
}

void WebLayout::addJs(QString resourcePath) {
    _jsResources << resourcePath;
}

void WebLayout::addWebWidget(WebWidget* webWidget) {
    _webWidgets << webWidget;
}

QString WebLayout::renderHtml(const Http::Request& request) {
    Html::Document document;
    document.setTitle(title());

    foreach(QString resource, _cssResources) {
        QDomElement linkNode = document.createElement("link");
        linkNode.setAttribute("href", resource);
        linkNode.setAttribute("rel", "stylesheet");
        linkNode.setAttribute("type", "text/css");
        document.head().appendChild(linkNode);
    }

    foreach(QString resource, _jsResources) {
        QDomElement scriptNode = document.createElement("script");
        scriptNode.setAttribute("src", resource);
        scriptNode.setAttribute("type", "text/javascript");

        // We have to put in this hack because self-closing script
        // tags are not allowed in HTML
        QDomText dummyText = document.createTextNode("");
        scriptNode.appendChild(dummyText);

        document.body().appendChild(scriptNode);
    }

    foreach(WebWidget *webWidget, _webWidgets) {
        document.appendHtml(
            document.body(),
            webWidget->renderHtml(request)
        );
    }

    return document.toString();
}

void WebLayout::setTitle(QString title) {
    _title = title;
}

QString WebLayout::title() {
    return _title;
}

} // namespace QtWebServer
