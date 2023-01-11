#ifndef FSLIB_ERRORGENERATOR_H
#define FSLIB_ERRORGENERATOR_H

#include <autil/Log.h>
#include <autil/Lock.h>
#include "fslib/fslib.h"
#include "fslib/fs/FileSystem.h"
#include "fslib/util/Singleton.h"

FSLIB_BEGIN_NAMESPACE(fs);

struct FileSystemError {
    FileSystemError()
        : ec(EC_OK)
        , offset(0)
        , delay(0)
        , retryCount(0)
        , until(0)
    {
    }
    ErrorCode ec;
    uint64_t offset;
    uint64_t delay;
    uint32_t retryCount;
    uint32_t until;
};
    
class ErrorGenerator : public util::Singleton<ErrorGenerator>
{
public:
    friend class util::LazyInstantiation;
    typedef std::map<std::pair<std::string, std::string>, FileSystemError> FileSystemErrorMap;
    typedef std::map<std::string, uint32_t> PathVisitCount;

public:
    typedef std::map<std::pair<std::string, std::string>, uint32_t> MethodVisitCount;

public:
    ~ErrorGenerator();

public:
    ErrorCode generateFileSystemError(const std::string& operate, 
            const std::string& dest);

    ErrorCode generateFileError(const std::string& operate, const std::string& targetPath,
                                uint64_t offset);
    static ErrorGenerator* getInstance();
    void clearMethodVisitCounter();
    MethodVisitCount getMethodVisitCounter();
    void setErrorMap(FileSystemErrorMap map);
    void resetPathVisitCount();

private:
    void init();
    bool parseErrorString(const std::string& errStr, std::string& method,
                          FileSystemError& fsError) const;
    void doDelay(uint64_t delay);
    bool needRetry(const std::string& parsePath, uint32_t retry);
    bool reachUntil(const std::string& parsePath, uint32_t until);
    FileSystemErrorMap::const_iterator getError(
            const std::string& operate, const std::string& parsePath);


private:
    ErrorGenerator();
    ErrorGenerator(const ErrorGenerator&);
    ErrorGenerator& operator=(const ErrorGenerator&);

private:
    friend class ErrorGeneratorTest;
private:
    FileSystemErrorMap _fileSystemErrorMap;
    PathVisitCount _pathVisitCount;
    MethodVisitCount _methodVisitCounter;
    autil::ThreadMutex mMutex;
};

FSLIB_TYPEDEF_AUTO_PTR(ErrorGenerator);

FSLIB_END_NAMESPACE(fs);

#endif //FSLIB_ERRORGENERATOR_H
