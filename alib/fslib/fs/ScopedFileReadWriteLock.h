#ifndef FSLIB_SCOPEDFILEREADWRITELOCK_H
#define FSLIB_SCOPEDFILEREADWRITELOCK_H

#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/FileReadWriteLock.h"

FSLIB_BEGIN_NAMESPACE(fs);

class ScopedFileReadWriteLock
{
public:
    friend class FileSystem;
    friend class ScopedFileReadWriteLockTest;

public:
    ScopedFileReadWriteLock();
    ~ScopedFileReadWriteLock();

private:
    void release();
    bool init(FileReadWriteLock* lock, const char mode);

private:
    FileReadWriteLock* _lock;
    char _mode;
};

FSLIB_TYPEDEF_AUTO_PTR(ScopedFileReadWriteLock);

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_SCOPEDFILEREADWRITELOCK_H
