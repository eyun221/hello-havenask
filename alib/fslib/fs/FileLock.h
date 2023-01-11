#ifndef FSLIB_FILELOCK_H
#define FSLIB_FILELOCK_H

#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"

FSLIB_BEGIN_NAMESPACE(fs);

class FileLock
{
public:
    virtual ~FileLock() {}

public:
    virtual int lock(uint32_t timeout = 0) = 0;
    virtual int unlock() = 0;
    
};

FSLIB_TYPEDEF_AUTO_PTR(FileLock);

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_FILELOCK_H
