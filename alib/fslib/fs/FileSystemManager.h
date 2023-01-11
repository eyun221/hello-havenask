#ifndef FSLIB_FILESYSTEMMANAGER_H
#define FSLIB_FILESYSTEMMANAGER_H

#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"

FSLIB_BEGIN_NAMESPACE(fs);

class AbstractFileSystem;

class FileSystemManager
{
public:
    static AbstractFileSystem* getFs(const FsType& type, bool safe = true);
    static void setFs(const FsType& type, AbstractFileSystem* fs, bool safe = true);

    static AbstractFileSystem* getRawFs(const FsType& type, bool safe = true);
    static void setRawFs(const FsType& type, AbstractFileSystem* fs, bool safe = true);

    static AbstractFileSystem* getRawFs(AbstractFileSystem* fs);
    static void destroyFs(const FsType& type);

    static void close();
    static bool init();
};

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_FILESYSTEMMANAGER_H
