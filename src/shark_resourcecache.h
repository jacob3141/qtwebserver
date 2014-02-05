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

#ifndef SHARK_RESOURCECACHE_H
#define SHARK_RESOURCECACHE_H

// Qt includes
#include <QString>
#include <QFileSystemWatcher>
#include <QObject>
#include <QMap>

namespace Shark {

class ResourceCache : public QObject {
    Q_OBJECT
public:
    enum CachingOption {
        Off    = 0x001,
        Minify = 0x002
    };

    Q_DECLARE_FLAGS(CachingOptions, CachingOption)

    struct Resource {
        QString contents;
        QString checksum;
    };

    ResourceCache(QString rootDirectory, QObject* parent = 0);

    QString read(QString uniqueResourceIdentifier);

    void setCachingOptions(CachingOptions cachingOptions);
    CachingOptions cachingOptions();

protected slots:
    void onFileChanged(QString filePath);
    void onDirectoryChanged(QString directoryPath);

protected:
    void updateCache(QString uniqueResourceIdentifier);
    void index();
    void indexRecursively(QString directory);

private:
    CachingOptions _cachingOptions;
    QMap<QString, Resource> _cache;
    QString _rootDirectory;
    QFileSystemWatcher _fileSystemWatcher;
};

} // namespace Shark

#endif // SHARK_RESOURCECACHE_H
