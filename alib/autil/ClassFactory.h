#ifndef AUTIL_CLASS_FACTORY_H
#define AUTIL_CLASS_FACTORY_H

#include <functional>
#include <map>
#include <vector>
#include <string>
#include <memory>

namespace autil {

template<typename KeyType, typename ClassType, typename InitType = int>
class ClassFactory {
    typedef std::function<ClassType*(InitType)> CreateFunc;
    struct CreateFuncEntry{
        CreateFunc func;
        int priority;
        KeyType key;
    };
    typedef std::map<KeyType, CreateFuncEntry> CreatorMap;
public:
    static ClassType* create(const KeyType &key, InitType value) {
        auto creatorMap = Global();
        if (creatorMap.find(key) == creatorMap.end()) {
            return nullptr;
        }
        return creatorMap[key].func(value);
    }
    static void createAll(std::vector<ClassType*> &container, InitType value) {
        auto creatorMap = Global();
        for (auto kv : creatorMap) {
            container.emplace_back(kv.second.func(value));
        }
    }
    static void createAll(std::vector<std::unique_ptr<ClassType>> &container,
                          InitType value)
    {
        auto creatorMap = Global();
        for (auto kv : creatorMap) {
            container.emplace_back(kv.second.func(value));
        }
    }
    static void createAll(std::vector<std::shared_ptr<ClassType>> &container,
                          InitType value)
    {
        auto creatorMap = Global();
        for (auto kv : creatorMap) {
            container.emplace_back(kv.second.func(value));
        }
    }
    static bool registerFunc(const KeyType &key, CreateFunc func, int priority) {
        // return bool only for init static variable
        CreateFuncEntry entry;
        entry.func = func;
        entry.key = key;
        entry.priority = priority;
        auto &creatorMap = Global();
        auto it = creatorMap.find(key);
        if (it == creatorMap.end()) {
            creatorMap[key] = entry;
        } else if (creatorMap[key].priority < priority) {
            // already exist an Entry, replace if have higher priority
            creatorMap[key] = entry;
        }
        return true;
    }
    static CreatorMap& Global() {
        static CreatorMap creatorMap;
        return creatorMap;
    }
};

/*
 * example:
 *    ClassA
 *    /    \
 * ClassB  ClassC
 *
 * COMMON_REGISTER_CLASS_TO_FACTORY(ClassA, ClassC, Default); will
 * register ClassC to key "Default", then we can use COMMON_CREATE_CLASS(ClassA, Default)
 * to get a ClassA* which point to ClassC object.
 */
#define COMMON_REGISTER_CLASS_TO_FACTORY(baseClass, subClass, key, priority) \
    /*evil marco trick to make sure __COUNTER__ is expanded*/           \
    COMMON_REGISTER_CLASS_TO_FACTORY_UNIQ(__COUNTER__, baseClass, subClass, key, priority)

#define COMMON_REGISTER_CLASS_TO_FACTORY_UNIQ(ctr, baseClass, subClass, key, priority) \
    COMMON_REGISTER_CLASS_TO_FACTORY_HELPER(ctr, baseClass, subClass, key, priority)

#define COMMON_REGISTER_CLASS_TO_FACTORY_HELPER(ctr, baseClass, subClass, key, priority) \
    static bool register_##ctr##_body                                   \
    = autil::ClassFactory<std::string, baseClass>::registerFunc(#key,   \
            [](const int &i){return (baseClass*)new subClass();}, priority) \

#define CUSTOM_REGISTER_CLASS_TO_FACTORY(baseClass, subClass, initClass, key, priority) \
    /*evil marco trick to make sure __COUNTER__ is expanded*/           \
    CUSTOM_REGISTER_CLASS_TO_FACTORY_UNIQ(__COUNTER__, baseClass,       \
            subClass, initClass, key, priority)

#define CUSTOM_REGISTER_CLASS_TO_FACTORY_UNIQ(ctr, baseClass, subClass, initClass, key, priority) \
    CUSTOM_REGISTER_CLASS_TO_FACTORY_HELPER(ctr, baseClass, subClass, initClass, key, priority)

#define CUSTOM_REGISTER_CLASS_TO_FACTORY_HELPER(ctr, baseClass, subClass, initClass, key, priority) \
    static bool register_##ctr##_body                                   \
    = autil::ClassFactory<std::string, baseClass, initClass>::registerFunc(#key, \
            [](initClass i){return (baseClass*)new subClass(i);}, priority) \


#define COMMON_CREATE_CLASS(baseClass, key)                             \
    autil::ClassFactory<std::string, baseClass>::create(#key, 0)

#define COMMON_CREATE_ALL_CLASS(baseClass, container)                   \
    autil::ClassFactory<std::string, baseClass>::createAll(container, 0)

#define CUSTOM_CREATE_CLASS(baseClass, initClass, key, initValue)       \
    autil::ClassFactory<std::string, baseClass, initClass>::create(#key, initValue)

#define CUSTOM_CREATE_ALL_CLASS(baseClass, initClass, container, initValue) \
    autil::ClassFactory<std::string, baseClass, initClass>::createAll(container, initValue)


}
#endif //AUTIL_CLASS_FACTORY_H
