#ifndef BEEPER_DLLWRAPPER_H
#define BEEPER_DLLWRAPPER_H

#include <autil/Log.h>
#include "beeper/common/common.h"
#include <dlfcn.h>
#include <string>

BP_NAMESPACE_BEGIN(recorder);

class DllWrapper
{
public:
    DllWrapper(const std::string &dllPath);
    ~DllWrapper();
    
public:
    bool dlopen();
    bool dlclose();
    void* dlsym(const std::string &symName);
    std::string dlerror();
    std::string getLocalLibPath();
    bool initLibFile();

private:
    std::string _dllPath;
    void *_handle;
    std::string _lastError;

private:
    BP_LOG_DECLARE();
};
typedef std::tr1::shared_ptr<DllWrapper> DllWrapperPtr;

//////////////////////////////////////////////////////////
struct ModuleInfo {
    ModuleInfo(const std::string& _path,
               uint32_t _version,
               const DllWrapperPtr& _dllWrapper)
        : path(_path)
        , majorVersion(_version)
        , dllWrapper(_dllWrapper)
    {}

    ~ModuleInfo()
    {
        if (dllWrapper)
        {
            dllWrapper->dlclose();
        }
    }
    
    std::string path;
    uint32_t majorVersion;
    DllWrapperPtr dllWrapper;
};
typedef std::tr1::shared_ptr<ModuleInfo> ModuleInfoPtr;

BP_NAMESPACE_END(recorder);

#endif //BEEPER_DLLWRAPPER_H

