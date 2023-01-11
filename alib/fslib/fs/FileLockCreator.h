#ifndef FSLIB_FILELOCKCREATOR_H
#define FSLIB_FILELOCKCREATOR_H

#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/FileReadWriteLock.h"
#include "fslib/fs/FileLock.h"

FSLIB_BEGIN_NAMESPACE(fs);

class FileLockCreator
{
public:
    virtual ~FileLockCreator() {}

public:
    virtual FileReadWriteLock* createFileReadWriteLock(
            const std::string& fileName) = 0;
    virtual FileLock* createFileLock(const std::string& fileName) = 0;
};

FSLIB_TYPEDEF_AUTO_PTR(FileLockCreator);

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_FILELOCKCREATOR_H
