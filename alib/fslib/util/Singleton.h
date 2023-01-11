#ifndef FSLIB_SINGLETON_H
#define FSLIB_SINGLETON_H

#include <autil/Log.h>
#include <autil/Lock.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"

FSLIB_BEGIN_NAMESPACE(util);

class LazyInstantiation
{
protected:
    template<typename T>
    static void create(T*& ptr)
    {
        ptr = new T;
        static std::unique_ptr<T> destroyer(ptr);
    }

};

template<typename T, typename InstPolicy = LazyInstantiation>
class Singleton : private InstPolicy
{
protected:
    Singleton(const Singleton&) {}
    Singleton(){}

public:
    ~Singleton(){}

public:
    /**
     * Provide access to the single instance through double-checked locking
     *
     * @return the single instance of object.
     */
    static T* getInstance();
};

template<typename T, typename InstPolicy>
T* Singleton<T, InstPolicy>::getInstance()
{
    static T* ptr = 0;
    static autil::ThreadMutex gLock;

    if (!ptr)
    {
        autil::ScopedLock lock(gLock);
        if (!ptr)
        {
            InstPolicy::create(ptr);
        }
    }
    return const_cast<T*>(ptr);
}

FSLIB_END_NAMESPACE(util);

#endif //FSLIB_SINGLETON_H
