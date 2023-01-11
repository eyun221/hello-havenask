#ifndef FSLIB_SCOPEDFILELOCK_H
#define FSLIB_SCOPEDFILELOCK_H

#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/FileLock.h"

FSLIB_BEGIN_NAMESPACE(fs);

class ScopedFileLock
{
public:
    friend class FileSystem;
    friend class ScopedFileLockTest;

public:
    ScopedFileLock();
    ~ScopedFileLock();

private:
    void release();
    bool init(FileLock* lock);

private:
    FileLock* _lock;
};

FSLIB_TYPEDEF_AUTO_PTR(ScopedFileLock);

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_SCOPEDFILELOCK_H
