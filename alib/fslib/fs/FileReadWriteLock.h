#ifndef FSLIB_FILEREADWRITELOCK_H
#define FSLIB_FILEREADWRITELOCK_H

#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"

FSLIB_BEGIN_NAMESPACE(fs);

class FileReadWriteLock
{
public:
    virtual ~FileReadWriteLock() {}

public:
    /**
     * rdlock request read lock for the file
     * @param timeout [in] timeout for waiting for the lock, default timeout is 0, 
     *                     0 means wait forever.
     * @return int, -1 indicates get lock fail, otherwise mean succeed
     */
    virtual int rdlock(uint32_t timeout = 0) = 0;

    /**
     * wrlock request write lock for the file
     * @param timeout [in] timeout for waiting for the lock, default timeout is 0, 
     *                     0 means wait forever.
     * @return int, -1 indicates get lock fail, otherwise mean succeed
     */
    virtual int wrlock(uint32_t timeout = 0) = 0;

    /**
     * unlock release the lock currently hold
     * @return int, -1 indicates release lock fail, otherwise mean succeed
     */
    virtual int unlock() = 0;
    
};

FSLIB_TYPEDEF_AUTO_PTR(FileReadWriteLock);

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_FILEREADWRITELOCK_H
