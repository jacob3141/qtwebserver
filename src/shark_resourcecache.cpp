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
#include "shark_resourcecache.h"

// Qt includes
#include <QDir>
#include <QFileInfo>
#include <QFileInfoList>
#include <QFile>

namespace Shark {

ResourceCache::ResourceCache(QString rootDirectory, QObject *parent)
    : QObject(parent),
      Logger("Shark::ResourceCache") {
    _rootDirectory = rootDirectory;
    index();

    connect(&_fileSystemWatcher, SIGNAL(fileChanged(QString)), this, SLOT(onFileChanged(QString)));
    connect(&_fileSystemWatcher, SIGNAL(directoryChanged(QString)), this, SLOT(onDirectoryChanged(QString)));
    _cachingOptions = ResourceCache::Off;
}

void ResourceCache::updateCache(QString uniqueResourceIdentifier) {
    log(QString("Updating cache for resource identifier \"%1\".").arg(uniqueResourceIdentifier));
    QString fileSystemPath = _rootDirectory + uniqueResourceIdentifier;
    QFile file(fileSystemPath);
    file.open(QFile::ReadOnly);
    if(file.isOpen()) {
        Resource resource;
        resource.contents = file.readAll();
        _cache[uniqueResourceIdentifier] = resource;
        file.close();
    }
}

void ResourceCache::index() {
    indexRecursively(_rootDirectory);
}

void ResourceCache::indexRecursively(QString directory) {
    _fileSystemWatcher.addPath(directory);
    QDir dir(_rootDirectory);
    QFileInfoList fileInfoList = dir.entryInfoList(QDir::Dirs | QDir::Files | QDir::NoDotAndDotDot);
    foreach(QFileInfo fileInfo, fileInfoList) {
        if(fileInfo.isDir()) {
            indexRecursively(fileInfo.absolutePath());
        } else {
            _fileSystemWatcher.addPath(fileInfo.absolutePath());
        }
    }
}

QString ResourceCache::read(QString uniqueResourceIdentifier) {
    if(!_cache.contains(uniqueResourceIdentifier) || _cachingOptions & ResourceCache::Off) {
        updateCache(uniqueResourceIdentifier);
    } else {
        log(QString("Cache hit for resource \"%1\".").arg(uniqueResourceIdentifier));
    }
    return _cache[uniqueResourceIdentifier].contents;
}

void ResourceCache::setCachingOptions(CachingOptions cachingOptions) {
    _cachingOptions = cachingOptions;
}

ResourceCache::CachingOptions ResourceCache::cachingOptions() {
    return _cachingOptions;
}

void ResourceCache::onFileChanged(QString filePath) {
    Q_UNUSED(filePath);
}

void ResourceCache::onDirectoryChanged(QString directoryPath) {
    Q_UNUSED(directoryPath);
}

}
