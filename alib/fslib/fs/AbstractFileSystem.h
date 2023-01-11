#ifndef FSLIB_ABSTRACTFILESYSTEM_H
#define FSLIB_ABSTRACTFILESYSTEM_H

#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"
#include "fslib/fs/File.h"
#include "fslib/fs/MMapFile.h"
#include "fslib/fs/FileReadWriteLock.h"
#include "fslib/fs/FileLock.h"

FSLIB_BEGIN_NAMESPACE(fs);

class AbstractFileSystem
{
public:
    virtual ~AbstractFileSystem() {}

public:
    virtual File* openFile(const std::string& fileName, Flag flag) = 0;
    // openFile with fileLength hint to avoid getFileMeta, -1 means unknown
    virtual File* openFile(const std::string& fileName, Flag flag, ssize_t fileLength) {
        return openFile(fileName, flag);
    }

    virtual MMapFile* mmapFile(const std::string& fileName, Flag openMode, 
                               char* start, size_t length, int prot, 
                               int mapFlag, off_t offset) = 0;
    // mmapFile with fileLength hint to avoid getFileMeta, -1 means unknown
    virtual MMapFile* mmapFile(const std::string& fileName, Flag openMode, 
                               char* start, size_t length, int prot, 
                               int mapFlag, off_t offset, ssize_t fileLength) {
        return mmapFile(fileName, openMode, start, length, prot, mapFlag, offset);
    }

    virtual ErrorCode rename(const std::string& oldFileName, const std::string& newFileName) = 0;
    virtual ErrorCode getFileMeta(const std::string& fileName, FileMeta& fileMeta) = 0;

    virtual ErrorCode isExist(const std::string& path) = 0;
    virtual ErrorCode isFile(const std::string& path) = 0;
    virtual ErrorCode isDirectory(const std::string& path) = 0;

    virtual ErrorCode mkDir(const std::string& dirName, bool recursive = false) = 0;
    
    virtual ErrorCode listDir(const std::string& dirName, FileList& fileList) = 0;
    virtual ErrorCode listDir(const std::string& dirName, EntryList& entryList) = 0;
    virtual ErrorCode listDir(const std::string& dirName, RichFileList& fileList) { return EC_NOTSUP; }

    // removeFile & removeDir superior to remove for pangu
    virtual ErrorCode remove(const std::string& path) = 0;
    virtual ErrorCode removeFile(const std::string& path) { return EC_NOTSUP; }
    virtual ErrorCode removeDir(const std::string& path) { return EC_NOTSUP; }

    virtual FileReadWriteLock* createFileReadWriteLock(
            const std::string& fileName) = 0;
    
    virtual FileLock* createFileLock(const std::string& fileName) = 0;
    
    virtual FileChecksum getFileChecksum(const std::string& fileName) = 0;
    
    virtual FileSystemCapability getCapability() const {
        return FSC_ALL_CAPABILITY;
    }

    virtual ErrorCode getPathMeta(const std::string &path, PathMeta &pathMeta) {
        ErrorCode ret = isFile(path);
        if (ret == EC_TRUE) {
            pathMeta.isFile = true;
        } else if (ret == EC_FALSE) {
            pathMeta.isFile = false;
        } else {
            return ret;
        }
        FileMeta fileMeta;
        ret = getFileMeta(path, fileMeta);
        if (ret == EC_OK) {
            pathMeta.length = fileMeta.fileLength;
            pathMeta.createTime = fileMeta.createTime;
            pathMeta.lastModifyTime = fileMeta.lastModifyTime;
        }
        return ret;
    }

    // forward command to real file system
    virtual ErrorCode forward(const std::string &command, const std::string &path,
                              const std::string &args, std::string &output) {
        return EC_NOTSUP;
    }
};

FSLIB_TYPEDEF_AUTO_PTR(AbstractFileSystem);

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_ABSTRACTFILESYSTEM_H
