#ifndef AUTIL_LEGACY_ANY_JSONIZABLE_H
#define AUTIL_LEGACY_ANY_JSONIZABLE_H

#include "autil/legacy/json.h"
#include "autil/legacy/base64.h"
#include "autil/legacy/jsonizable_exception.h"
#include "autil/legacy/jsonizable_dec.h"
#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <sstream>
#include <tr1/memory>
#include <tr1/type_traits>
#include "autil/LongHashValue.h"

namespace autil{ namespace legacy
{

// Usage:
// struct S : public Jsonizable
// {
//     int n;
//     string s;

//     /* override */ void Jsonize(JsonWrapper& json)
//     {
//         json.Jsonize("n", n);
//         json.Jsonize("s", s, "default_value");
//     }
// };

struct arithmetic_tag {};
struct enum_tag {};

template<typename T, 
         bool = std::tr1::is_enum<T>::value,
         bool = std::tr1::is_arithmetic<T>::value>
struct dispatch_trait;

template<typename T>
struct dispatch_trait<T, true, false> {
    typedef enum_tag tag_type;
};

template<typename T>
struct dispatch_trait<T, false, true> {
    typedef arithmetic_tag tag_type;
};

template<typename T, typename if_exist>
struct enable_if_exist {
    typedef T type;
};

template<typename T>
inline typename enable_if_exist<Any, typename dispatch_trait<T>::tag_type>::type
ToJson(const T& i);

template<typename T>
inline typename enable_if_exist<void, typename dispatch_trait<T>::tag_type>::type
FromJson(T& i, const Any& a);

inline Any ToJson(const Any& a);
inline void FromJson(Any& t, const Any& a);
inline Any ToJson(const std::string& s);
inline void FromJson(std::string& s, const Any& a);
inline Any ToJson(const char* s);
inline Any ToJson(const ExceptionBase& e);
inline void FromJson(ExceptionBase& e, const Any& a);
inline Any ToJson(const uint128_t &uint128);
inline void FromJson(uint128_t &uint128, const Any& a);

template<typename T>
Any ToJson(T* p);

template<typename T>
void FromJson(T*& p, const Any& a);

template<typename T>
Any ToJson(const std::tr1::shared_ptr<T>& p);

template<typename T>
void FromJson(std::tr1::shared_ptr<T>& p, const Any& a);

template<typename T>
Any ToJson(const std::vector<T>& m);

template<typename T>
void FromJson(std::vector<T>& tVec, const Any& a);

template<typename T>
Any ToJson(const std::deque<T>& m);

template<typename T>
void FromJson(std::deque<T>& tVec, const Any& a);

template<typename T, typename C>
Any ToJson(const std::set<T, C>& s);

template<typename T, typename C>
void FromJson(std::set<T, C>& tSet, const Any& a);

template<typename T>
Any ToJson(const std::list<T>& s);

template<typename T>
void FromJson(std::list<T>& tSet, const Any& a);


template<typename T, typename U>
Any ToJson(const std::pair<T, U>& s);

// NO FromJson(std::pair<T, U>& tPair, const Any& a); because it's not define how to convert from Any

template<typename T, typename U, typename C>
Any ToJson(const std::map<T, U, C>& m);

template<typename T, typename U, typename C>
void FromJson(std::map<T, U, C>& tMap, const Any& a); //because it's not define how to convert from Any

template<typename T, typename U, typename C>
Any ToJson(const std::multimap<T, U, C>& m);

template<typename T, typename U, typename C>
void FromJson(std::multimap<T, U, C>& tMap, const Any& a); //because it's not define how to convert from Any

template<typename T, typename C>
Any ToJson(const std::map<std::string, T, C>& m);

template<typename T, typename C>
void FromJson(std::map<std::string, T, C>& tMap, const Any& a);

template<typename T>
inline typename enable_if_exist<Any, typename dispatch_trait<T>::tag_type>::type
ToJson(const T& i)
{
    return ToJson(i, typename dispatch_trait<T>::tag_type());
}

template<typename T>
inline typename enable_if_exist<void, typename dispatch_trait<T>::tag_type>::type
FromJson(T& i, const Any& a)
{
    FromJson(i, a, typename dispatch_trait<T>::tag_type());
}

template<typename T>
inline Any ToJson(const T& i, arithmetic_tag)
{
    return i;
}

template<typename T>
inline void FromJson(T& i, const Any& a, arithmetic_tag)
{
    i = json::JsonNumberCast<T>(a);
}

template<typename T>
inline Any ToJson(const T& i, enum_tag)
{
    return (int)i;
}

template<typename T>
inline void FromJson(T& i, const Any& a, enum_tag)
{
    i = static_cast<T>(json::JsonNumberCast<int>(a));
}

inline Any ToJson(const Any& a)
{
    return a;
}

inline void FromJson(Any& t, const Any& a)
{
    t = a;
}

inline Any ToJson(const Jsonizable& t)
{
    Jsonizable::JsonWrapper w;
    const_cast<Jsonizable&>(t).Jsonize(w);
    return w.GetMap();
}

inline void FromJson(Jsonizable& t, const Any& a)
{
    Jsonizable::JsonWrapper w(a);
    t.Jsonize(w);
}

inline Any ToJson(const std::string& s)
{
    return s;
}

inline void FromJson(std::string& s, const Any& a)
{
    s = AnyCast<std::string>(a);
}

inline Any ToJson(const char* s)
{
    return std::string(s);
}

inline Any ToJson(const uint128_t& primaryKey) {
    std::vector<Any> vec;
    vec.push_back(primaryKey.value[0]);
    vec.push_back(primaryKey.value[1]);

    return vec;
}

inline void FromJson(uint128_t& primaryKey, const Any& a)
{   
    std::vector<Any> vec = AnyCast<std::vector<Any> >(a);
    primaryKey.value[0] = json::JsonNumberCast<uint64_t>(vec[0]);
    primaryKey.value[1] = json::JsonNumberCast<uint64_t>(vec[1]);
}

template<typename T>
Any ToJson(const std::vector<T>& m)
{
    std::vector<Any> anyVec;
    for (typename std::vector<T>::const_iterator it = m.begin();
         it != m.end();
         it++)
    {
        anyVec.push_back(ToJson(*it));
    }
    return anyVec;
}

template<typename T>
void FromJson(std::vector<T>& tVec, const Any& a)
{
    tVec.clear();
    std::vector<Any> anyVec = AnyCast<std::vector<Any> >(a);
    for (typename std::vector<Any>::const_iterator it = anyVec.begin();
         it != anyVec.end();
         it++)
    {
        T t;
        FromJson(t, *it);
        tVec.push_back(t);
    }
}

template<typename T>
Any ToJson(const std::deque<T>& m)
{
    std::vector<Any> anyVec;
    for (typename std::deque<T>::const_iterator it = m.begin();
         it != m.end();
         it++)
    {
        anyVec.push_back(ToJson(*it));
    }
    return anyVec;
}

template<typename T>
void FromJson(std::deque<T>& tDeque, const Any& a)
{
    tDeque.clear();
    std::vector<Any> anyVec = AnyCast<std::vector<Any> >(a);
    for (typename std::vector<Any>::const_iterator it = anyVec.begin();
         it != anyVec.end();
         it++)
    {
        T t;
        FromJson(t, *it);
        tDeque.push_back(t);
    }
}

template<typename T>
Any ToJson(const std::list<T>& m)
{
    std::vector<Any> anyVec;
    for (typename std::list<T>::const_iterator it = m.begin();
         it != m.end();
         it++)
    {
        anyVec.push_back(ToJson(*it));
    }
    return anyVec;
}

template<typename T>
void FromJson(std::list<T>& tList, const Any& a)
{
    tList.clear();
    std::vector<Any> anyVec = AnyCast<std::vector<Any> >(a);
    for (typename std::vector<Any>::const_iterator it = anyVec.begin();
         it != anyVec.end();
         it++)
    {
        T t;
        FromJson(t, *it);
        tList.push_back(t);
    }
}

template<typename T, typename U>
Any ToJson(const std::pair<T, U>& s)
{
    std::vector<Any> anyVec;
    anyVec.push_back(ToJson(s.first));
    anyVec.push_back(ToJson(s.second));
    return anyVec;
}

template<typename T, typename U>
void FromJson(std::pair<T, U>& s, const Any& a)
{
    std::vector<Any> anyVec;
    FromJson(anyVec, a);
    if (anyVec.size() != 2)
        AUTIL_LEGACY_THROW(autil::legacy::NotJsonizableException,
                     "FromJson(std::pair<T, U>& s, const Any& a): vector.size()!=2");
    FromJson(s.first, anyVec[0]);
    FromJson(s.second, anyVec[1]);
}

template<typename T, typename U, typename C>
Any ToJson(const std::map<T, U, C>& m)
{
    std::vector<Any> anyVec;
    std::vector<std::pair<T, U> > vec(m.begin(), m.end());

    for (typename std::vector<std::pair<T, U> >::const_iterator it = vec.begin();
         it != vec.end();
         it++)
    {
        anyVec.push_back(ToJson(*it));
    }

    return anyVec;
}

template<typename T, typename U, typename C>
Any ToJson(const std::multimap<T, U, C>& m)
{
    std::vector<Any> anyVec;
    std::vector<std::pair<T, U> > vec(m.begin(), m.end());

    for (typename std::vector<std::pair<T, U> >::const_iterator it = vec.begin();
         it != vec.end();
         it++)
    {
        anyVec.push_back(ToJson(*it));
    }

    return anyVec;
}

template<typename T, typename U, typename C>
void FromJson(std::map<T, U, C>& m, const Any& a)
{
    const std::vector<Any>& anyVec = AnyCast<std::vector<Any> >(a);
    m.clear();
    typename std::map<T, U, C>::iterator insPos = m.begin();
    for (typename std::vector<Any>::const_iterator it = anyVec.begin();
         it != anyVec.end();
         it++)
    {
        std::pair<T, U> p;
        FromJson(p, *it);
        insPos = m.insert(insPos, p);
    }
}

template<typename T, typename U, typename C>
void FromJson(std::multimap<T, U, C>& m, const Any& a)
{
    const std::vector<Any>& anyVec = AnyCast<std::vector<Any> >(a);
    m.clear();
    typename std::multimap<T, U, C>::iterator insPos = m.begin();
    for (typename std::vector<Any>::const_iterator it = anyVec.begin();
         it != anyVec.end();
         it++)
    {
        std::pair<T, U> p;
        FromJson(p, *it);
        insPos = m.insert(insPos, p);
        ++ insPos;
    }
}

template<typename T, typename C>
Any ToJson(const std::map<std::string, T, C>& m)
{
    std::map<std::string, Any> anyMap;
    std::map<std::string, Any>::iterator insPos = anyMap.begin();
    for (typename std::map<std::string, T>::const_iterator it = m.begin();
        it != m.end();
        it++)
    {
        insPos = anyMap.insert(insPos, std::make_pair(it->first, ToJson(it->second)));
        //anyMap[it->first] = ToJson(it->second);
    }
    return anyMap;
}

template<typename T, typename C>
void FromJson(std::map<std::string, T, C>& tMap, const Any& a)
{
    tMap.clear();
    typename std::map<std::string, T, C>::iterator insPos = tMap.begin();
    const std::map<std::string, Any>& anyMap = AnyCast<std::map<std::string, Any> >(a);
    for (typename std::map<std::string, Any>::const_iterator it = anyMap.begin();
         it != anyMap.end();
         it++)
    {
        std::pair<std::string, T> p;
        p.first = it->first;
        FromJson(p.second, it->second);
        insPos = tMap.insert(insPos, p);
    }
}

template<typename T, typename C>
Any ToJson(const std::set<T, C>& m)
{
    std::vector<T> anyVec(m.begin(), m.end());
    return ToJson(anyVec);
}

template<typename T, typename C>
void FromJson(std::set<T, C>& tSet, const Any& a)
{
    tSet.clear();
    typename std::set<T, C>::iterator insPos = tSet.begin();
    const std::vector<Any>& anyVec = AnyCast<std::vector<Any> >(a);
    for (typename std::vector<Any>::const_iterator it = anyVec.begin();
         it != anyVec.end();
         it++)
    {
        T t;
        FromJson(t, *it);
        insPos = tSet.insert(insPos, t);
    }
}

template<typename T>
Any ToJson(T* p)
{
    if (p)
        return ToJson(*p);
    else
        return Any();
}

template<typename T>
void FromJson(T*& p, const Any& a)
{
    if (a.GetType() == typeid(void))
        p = NULL;
    else
    {
#if __cplusplus >= 201103L
        std::unique_ptr<T> t(new T);
#else        
        std::auto_ptr<T> t(new T);
#endif
        FromJson(*t, a);
        p = t.release();
    }
}

template<typename T>
Any ToJson(const std::tr1::shared_ptr<T>& p)
{
    return ToJson(p.get());
}

template<typename T>
void FromJson(std::tr1::shared_ptr<T>& p, const Any& a)
{
    T *t = NULL;
    FromJson(t, a);
    p.reset(t);
}

template<typename T>
Any ToJson(const std::shared_ptr<T>& p)
{
    return ToJson(p.get());
}

template<typename T>
void FromJson(std::shared_ptr<T>& p, const Any& a)
{
    T *t = NULL;
    FromJson(t, a);
    p.reset(t);
}

template<typename T>
std::string ToJsonString(const T& t)
{
    return autil::legacy::json::ToString(ToJson(t));
}

template<typename T>
std::string ToJsonString(const T& t, bool isCompact,
                         const std::string &prefix = "")
{
    return autil::legacy::json::ToString(ToJson(t), isCompact, prefix);
}

template<typename T>
void FromJsonString(T& t, const std::string& str)
{
    Any a = json::ParseJson(str);
    FromJson(t, a);
}

inline Any ToJson(const ExceptionBase& e)
{
    json::JsonMap m;
    m["Message"] = e.mMessage;
    m["File"] = std::string(e.mFile);
    m["Function"] = std::string(e.mFunction);
    m["Line"] = e.mLine;
    m["ClassName"] = e.GetClassName();
    if (e.mNestedException)
        m["Cause"] = ToJson(*e.mNestedException);
    return m;
}

inline void FromJson(ExceptionBase& e, const Any& a)
{
    json::JsonMap am = AnyCast<json::JsonMap>(a);
    e.mMessage = AnyCast<std::string>(am["Message"]);
    if (am.find("Cause") != am.end())
    {
        ExceptionBase cause;
        FromJson(cause, am["Cause"]);
        e.SetCause(cause);
    }
}

}}//namespace autil::legacy

#endif//AUTIL_LEGACY_JSONIZABLE_H
