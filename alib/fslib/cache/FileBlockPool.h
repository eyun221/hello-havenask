#ifndef FSLIB_FILEBLOCKPOOL_H
#define FSLIB_FILEBLOCKPOOL_H

#include <autil/Log.h>
#include <autil/Lock.h>
#include "fslib/fslib.h"
#include "fslib/fs/FileSystem.h"

FSLIB_BEGIN_NAMESPACE(cache);

class FileBlockPool
{
public:
    FileBlockPool(int64_t bufferSize = DEFAULT_MEM_POOL_SIZE, 
                  size_t blockSize = DEFAULT_FILE_BLOCK_SIZE,
                  bool forceAllocate = true);
    ~FileBlockPool();
    
public:
    int32_t allocate(void* &buffer);
    void deAllocate(void* &buffer);

    size_t getMaxBlockCount() const {return _maxBlockCount;}
    size_t getBlockSize() const {return _blockSize;}

    size_t getBlockCount() const {
        autil::ScopedLock lock(_mutex);
        return _blockCount;
    }
    int64_t getLeftSize() const {
        autil::ScopedLock lock(_mutex);
        return (_maxBlockCount - _blockCount) * _blockSize;
    }
    int64_t getUsedSize() const {
        autil::ScopedLock lock(_mutex);
        return _blockCount * _blockSize;
    }
private:
    mutable autil::ThreadMutex _mutex;
    bool _forceAllocate;
    size_t _maxBlockCount;
    size_t _blockSize;
    size_t _blockCount;
    std::vector<void *> _bufferVec;
private:
    friend class FileBlockPoolTest;
};

FSLIB_TYPEDEF_AUTO_PTR(FileBlockPool);

FSLIB_END_NAMESPACE(cache);

#endif //FSLIB_FILEBLOCKPOOL_H
