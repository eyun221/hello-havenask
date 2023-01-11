#ifndef AUTIL_STACKTRACER_H
#define AUTIL_STACKTRACER_H

#include <tr1/memory>
#include <string>
#include <unordered_map>
#include <autil/CommonMacros.h>
#include <autil/Lock.h>
#include <autil/Log.h>
#include <autil/StringUtil.h>
#include <unistd.h>

AUTIL_BEGIN_NAMESPACE(autil);

#define STACK_TRACER_LOG(level, format, args...)                        \
    if (autil::StackTracer::UsingStackTracerLog())                      \
    {                                                                   \
        AUTIL_LOG(level, "StackTracerId[%lu], "#format,                 \
                  autil::StackTracer::getInstance()->getTraceId(), ##args); \
    }
    
#define DECLARE_STACK_TRACER_FILE(fileName)                             \
    char* envStr = getenv("STACK_TRACER_LOG");                          \
    if (envStr && std::string(envStr) == "true")                        \
    {                                                                   \
        char* stackDepthStr = getenv("STACK_TRACER_MAX_DEPTH");         \
        size_t maxDepth = 10;                                           \
        if (stackDepthStr && autil::StringUtil::fromString(             \
                        std::string(stackDepthStr), maxDepth))          \
        {                                                               \
            autil::StackTracer::getInstance()->setMaxDepth(maxDepth);   \
        }                                                               \
        autil::StackTracer::EnableStackTracerLog();                     \
        std::string fileNameString = fileName;                          \
        if (!fileNameString.empty()) {                                  \
            fileNameString += "." + autil::StringUtil::toString((int64_t)getpid()); \
            autil::StackTracer::getInstance()->initFile(fileNameString); \
        }                                                               \
    }

class StackTracer
{
public:
    class Creator
    {
    public:
        static StackTracer* create()
        {
            StackTracer* ptr = new StackTracer();
            static std::tr1::shared_ptr<StackTracer> destroyer(ptr);
            return ptr;
        }
    };
    
public:
    StackTracer() : _maxDepth(15), _fp(NULL) {}
    ~StackTracer() { if (_fp) { fclose(_fp); } }
    
private:
    StackTracer(const StackTracer &);
    StackTracer& operator = (const StackTracer &);
    
public:
    static StackTracer* getInstance()
    {
        if (unlikely(!ptr))
        {
            ScopedLock sl(gLock);
            if (!ptr)
            {
                ptr = Creator::create();
            }
        }
        return const_cast<StackTracer*>(ptr);
    }
    
    size_t getTraceId() const;
    void setMaxDepth(size_t maxDepth);
    bool initFile(const std::string& fileName);

    static bool UsingStackTracerLog() { 
        return mUseStackTracerLog;
    }
    
    static void EnableStackTracerLog() {
        mUseStackTracerLog = true;
    }

private:
    std::string getSubString(const std::string& str, char begin, char end) const
    {
        size_t startPos = str.find(begin);
        size_t endPos = str.find(end);
        return str.substr(startPos + 1, endPos - startPos - 1);
    }
    
private:
    volatile size_t _maxDepth;
    mutable std::unordered_map<std::string, int32_t> _traceIdMap;
    mutable ThreadMutex _lock;
    FILE* _fp;
    
    static bool mUseStackTracerLog;
    static StackTracer* ptr;
    static RecursiveThreadMutex gLock;
};

/////////////////////////////////////////////////////////////

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_STACKTRACER_H
