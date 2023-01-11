#ifndef __BEEPER_PLUGINEVENTRECORDER_H
#define __BEEPER_PLUGINEVENTRECORDER_H

#include <tr1/memory>
#include <autil/Lock.h>
#include "beeper/common/common.h"
#include "beeper/recorder/EventRecorder.h"
#include "beeper/recorder/DllWrapper.h"

BP_NAMESPACE_BEGIN(recorder);

class PluginEventRecorder : public EventRecorder
{
public:
    PluginEventRecorder(const std::string& identifier);
    ~PluginEventRecorder();
    
public:
    bool Init(const std::map<std::string, std::string>& kvParams) override
    {
        // not called
        assert(false);
        return false;
    }
    
    void Init(const DllWrapperPtr& dllWrapper,
              const std::map<std::string, std::string>& kvParams);
        
    void Close() override;
    
    /* Attention: Process interface should be multi-thread safe */
    bool Process(EventLevel level, const EventPtr& event) override;
    bool BatchProcess(EventLevel level, const std::vector<EventPtr>& events);
    
private:
    void CreateRecorder();
    void DestroyRecorder();
    
private:
    EventRecorder* mDllRecorder;
    std::map<std::string, std::string> mParameters;
    DllWrapperPtr mDllWrapper;
    autil::RecursiveThreadMutex mLock;
    
private:
    BP_LOG_DECLARE();
};

typedef std::tr1::shared_ptr<PluginEventRecorder> PluginEventRecorderPtr;

BP_NAMESPACE_END(recorder);

#endif //__BEEPER_PLUGINEVENTRECORDER_H
