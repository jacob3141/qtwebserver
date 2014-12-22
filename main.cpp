//
// Copyright 2010-2015 Jacob Dawid <jacob@omg-it.works>
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

// Qt includes
#include <QCoreApplication>

// Own includes
#include "Application.h"
#include "NetworkService.h"

int main(int argc, char *argv[]) {
    QCoreApplication a(argc, argv);

    // Create our Shark application
    Shark::Application *application = new Shark::Application("../applications/demo");

    // Set up a web service
    Shark::NetworkService *service = new Shark::NetworkService();

    // Configure our application to respond through the web service
    service->setHttpResponder(application);
    service->initialize();
    return a.exec();
}

