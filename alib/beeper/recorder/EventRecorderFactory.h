#ifndef __BEEPER_EVENTRECORDERFACTORY_H
#define __BEEPER_EVENTRECORDERFACTORY_H

#include <tr1/memory>
#include "beeper/common/common.h"
#include "beeper/recorder/EventRecorder.h"
#include "beeper/recorder/DllWrapper.h"

BP_DECLARE_REFERENCE_CLASS(config, BeeperGlobalConfig);

BP_NAMESPACE_BEGIN(recorder);

class EventRecorderFactory
{
public:
    EventRecorderFactory();
    ~EventRecorderFactory();
    
public:
    void Init(const config::BeeperGlobalConfigPtr& conf);
    
    EventRecorder* Create(const std::string& id, const std::string& type,
                          const std::map<std::string, std::string>& kvParams);

private:
    void ParseSoUnderOnePath(const config::BeeperGlobalConfigPtr& conf,
                             const std::string &path);
    
    bool ParsePlugin(const std::string &fileName, std::string &type,
                     uint32_t &majorVersion);

    static bool IsBuiltinRecorderType(const std::string& type);

    static bool IsNeededPluginType(
            const config::BeeperGlobalConfigPtr& conf,
            const std::string& type);
    
    EventRecorder* CreateBuiltinRecorder(
            const std::string& id, const std::string& type);
    
private:
    std::map<std::string, ModuleInfoPtr> mModuleInfoMap;
    
private:
    BP_LOG_DECLARE();
};

typedef std::tr1::shared_ptr<EventRecorderFactory> EventRecorderFactoryPtr;

BP_NAMESPACE_END(recorder);

#endif //__BEEPER_EVENTRECORDERFACTORY_H
