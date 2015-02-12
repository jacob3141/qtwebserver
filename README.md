[![Qt Pods](http://qt-pods.org/assets/logo.png "Qt Pods")](http://qt-pods.org)

# QtWebServer

QtWebServer is an attempt to create a high-performance Qt based web application server, ie. a web server that runs native C++/Qt code in order to deliver websites.

![Concept](https://github.com/cybercatalyst/qtwebserver/blob/master/qtwebserver.png "Concept")

Within the Qt app, you set up resources and bind them to physical providers, for example files or a database content. A web application object holds information on how to match uri patterns to resources, which again is accessed by a multithreaded tcp server component. The multithreaded server is an extension of Qt's QTcpServer class.

# Installation and usage

You can either integrate QtWebServer into your existing application or build a whole standalone web application. There is no distinction between the webserver and the web application, you literally link your web application with your website.

Installation via qt-pods is perfectly suited for this use-case. You can update your QtWebServer version at any time and switch back flawlessly, since it will be embedded as a git submodule. It is not recommended to plain copy the QtWebServer code into your target application. In order to learn more about qt-pods, see here:

https://github.com/cybercatalyst/qt-pods
