#ifndef AUTIL_LEGACY_FAST_JSONIZABLE_H
#define AUTIL_LEGACY_FAST_JSONIZABLE_H

#ifndef RAPIDJSON_HAS_STDSTRING
#define RAPIDJSON_HAS_STDSTRING 1
#endif

#if defined(__SSE4_2__)
#  ifndef AUTIL_RAPIDJSON_SSE42
#    define AUTIL_RAPIDJSON_SSE42
#  endif
#elif defined(__SSE2__)
#  ifndef AUTIL_RAPIDJSON_SSE2
#    define AUTIL_RAPIDJSON_SSE2
#  endif
#endif

#include <vector>
#include <deque>
#include <list>
#include <set>
#include <map>
#include <stack>
#include <sstream>
#include <tr1/memory>
#include <tr1/type_traits>
#include <iostream>
#include "autil/legacy/jsonizable_exception.h"
#include "autil/legacy/jsonizable_dec.h"
#include "autil/legacy/any.h"

#include "autil/legacy/rapidjson/document.h"
#include "autil/legacy/rapidjson/writer.h"
#include "autil/legacy/rapidjson/stringbuffer.h"
#include "autil/legacy/rapidjson/error/en.h"

namespace autil {
namespace legacy {

template <bool b, class T = void>
using enable_if_t = typename std::enable_if<b, T>::type;

// serializeToWriter declare begin
inline void serializeToWriter(RapidWriter *writer,
                              const std::string& t);
inline void serializeToWriter(RapidWriter *writer,
                              const char* t);
inline void serializeToWriter(RapidWriter *writer,
                              const Jsonizable &t);
inline void serializeToWriter(RapidWriter *writer,
                              bool t);
template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_signed<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t);
template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t);
template <typename T>
inline enable_if_t<std::is_enum<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t);
template <typename T>
inline enable_if_t<std::is_floating_point<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t);
template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::vector<T>& t);
template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::set<T>& t);
template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::deque<T>& t);
template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::list<T>& t);
template <typename T, typename C>
inline void serializeToWriter(RapidWriter *writer,
                              const std::map<std::string, T, C>& t);
template <typename T, typename U, typename C>
inline void serializeToWriter(RapidWriter *writer,
                              const std::map<T, U, C>& t);
template <typename T, typename U, typename C>
inline void serializeToWriter(RapidWriter *writer,
                              const std::multimap<T, U, C>& t);
template <typename T, typename U>
inline void serializeToWriter(RapidWriter *writer,
                              const std::pair<T, U>& t);
template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const T* t);
template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::tr1::shared_ptr<T>& t);
template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::shared_ptr<T>& t);
inline void serializeToWriter(RapidWriter *writer,
                              const ExceptionBase& t);
inline void serializeToWriter(RapidWriter *writer,
                              const uint128_t& t);

#include "autil/legacy/fast_jsonizable_any.h"

// serializeToWriter declare end

// FromRapidValue declare begin
inline void FromRapidValue(Jsonizable& t, RapidValue& value);
inline void FromRapidValue(std::string& t, RapidValue& value);
inline void FromRapidValue(bool& t, RapidValue& value);
template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_signed<T>::value, void>
    FromRapidValue(T& t, RapidValue& value);
template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value, void>
    FromRapidValue(T& t, RapidValue& value);
template <typename T>
inline enable_if_t<std::is_enum<T>::value, void>
FromRapidValue(T& t, RapidValue& value);
template <typename T>
inline enable_if_t<std::is_floating_point<T>::value, void>
FromRapidValue(T& t, RapidValue& value);
template <typename T>
inline void FromRapidValue(std::vector<T>& t, RapidValue& value);
template <typename T>
inline void FromRapidValue(std::set<T>& t, RapidValue& value);
template <typename T>
inline void FromRapidValue(std::deque<T>& t, RapidValue& value);
template <typename T>
inline void FromRapidValue(std::list<T>& t, RapidValue& value);
template <typename T, typename C>
inline void FromRapidValue(std::map<std::string, T, C>& t, RapidValue& value);
template <typename T, typename U, typename C>
inline void FromRapidValue(std::map<T, U, C>& t, RapidValue& value);
template <typename T, typename U, typename C>
inline void FromRapidValue(std::multimap<T, U, C>& t, RapidValue& value);
template <typename T, typename U>
inline void FromRapidValue(std::pair<T, U>& t, RapidValue& value);
template <typename T>
inline void FromRapidValue(T*& t, RapidValue& value);
template <typename T>
inline void FromRapidValue(std::tr1::shared_ptr<T>& t, RapidValue& value);
template <typename T>
inline void FromRapidValue(std::shared_ptr<T>& t, RapidValue& value);
inline void FromRapidValue(ExceptionBase& t, RapidValue& value);
inline void FromRapidValue(uint128_t& t, RapidValue& value);
// unimplemented
inline void FromRapidValue(Any& t, RapidValue& value) {
    AUTIL_LEGACY_THROW(NotSupportException, "not support.");
}
// FromRapidValue declare end


// serializeToWriter implemention begin
inline void serializeToWriter(RapidWriter *writer,
                              const std::string& t)
{
    writer->String(t);
}

inline void serializeToWriter(RapidWriter *writer,
                              const char* t)
{
    writer->String(t);
}

inline void serializeToWriter(RapidWriter *writer,
                              const Jsonizable &t)
{
    Jsonizable::JsonWrapper w(writer);
    writer->StartObject();
    const_cast<Jsonizable&>(t).Jsonize(w);
    writer->EndObject();
}

inline void serializeToWriter(RapidWriter *writer,
                              bool t)
{
    writer->Bool(t);
}

template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_signed<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t)
{
    writer->Int64((int64_t)t);
}

template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t)
{
    writer->Uint64((uint64_t)t);
}

template <typename T>
inline enable_if_t<std::is_enum<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t)
{
    writer->Int((int32_t)t);
}

template <typename T>
inline enable_if_t<std::is_floating_point<T>::value, void>
serializeToWriter(RapidWriter *writer,
                  const T& t)
{
    writer->Double((double)t);
}

#define SERIALIZE_AS_ARRAY(container, writer) \
    {                                         \
        writer->StartArray();                 \
        for (auto &v : container) {           \
            serializeToWriter(writer, v);     \
        }                                     \
        writer->EndArray();                   \
    }

template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::vector<T>& t)
{
    SERIALIZE_AS_ARRAY(t, writer)
}

template <>
inline void serializeToWriter<bool>(RapidWriter *writer,
                                    const std::vector<bool>& t)
{
    writer->StartArray();
    for (bool v : t) {
        serializeToWriter(writer, v);
    }
    writer->EndArray();
}

template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::set<T>& t)
{
    SERIALIZE_AS_ARRAY(t, writer)
}

template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::deque<T>& t)
{
    SERIALIZE_AS_ARRAY(t, writer)
}

template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::list<T>& t)
{
    SERIALIZE_AS_ARRAY(t, writer)
}

template <typename T, typename C>
inline void serializeToWriter(RapidWriter *writer,
                              const std::map<std::string, T, C>& t)
{
    writer->StartObject();
    for (auto &kv : t) {
        writer->Key(kv.first.c_str());
        serializeToWriter(writer, kv.second);
    }
    writer->EndObject();
}

template <typename T, typename U, typename C>
inline void serializeToWriter(RapidWriter *writer,
                              const std::map<T, U, C>& t)
{
    writer->StartArray();
    for (auto &kv : t) {
        serializeToWriter(writer, kv);
    }
    writer->EndArray();
}

template <typename T, typename U, typename C>
inline void serializeToWriter(RapidWriter *writer,
                              const std::multimap<T, U, C>& t)
{
    writer->StartArray();
    for (auto &kv : t) {
        serializeToWriter(writer, kv);
    }
    writer->EndArray();
}


template <typename T, typename U>
inline void serializeToWriter(RapidWriter *writer,
                              const std::pair<T, U>& t)
{
    writer->StartArray();
    serializeToWriter(writer, t.first);
    serializeToWriter(writer, t.second);
    writer->EndArray();
}

template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const T* t)
{
    if (t) {
        serializeToWriter(writer, *t);
    } else {
        writer->Null();
    }
}

template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::tr1::shared_ptr<T>& t)
{
    serializeToWriter(writer, t.get());
}

template <typename T>
inline void serializeToWriter(RapidWriter *writer,
                              const std::shared_ptr<T>& t)
{
    serializeToWriter(writer, t.get());
}

inline void serializeToWriter(RapidWriter *writer,
                              const ExceptionBase& t)
{
    writer->StartObject();
    writer->Key("Message");
    writer->String(t.mMessage);
    writer->Key("File");
    writer->String(t.mFile);
    writer->Key("Function");
    writer->String(t.mFunction);
    writer->Key("Line");
    writer->Int(t.mLine);
    writer->Key("ClassName");
    writer->String(t.GetClassName());
    if (t.mNestedException) {
        writer->Key("Cause");
        serializeToWriter(writer, t.mNestedException);
    }
    writer->EndObject();
}

inline void serializeToWriter(RapidWriter *writer,
                              const uint128_t& t)
{
    writer->StartArray();
    writer->Int64(t.value[0]);
    writer->Int64(t.value[1]);
    writer->EndArray();
}

// serializeToWriter implemention end

// FromRapidValue implemention begin
inline void FromRapidValue(Jsonizable& t, RapidValue& value) {
    if (!value.IsObject()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect Object but get .");
    }
    Jsonizable::JsonWrapper w(&value);
    t.Jsonize(w);
}

inline void FromRapidValue(std::string& t, RapidValue& value) {
    if (!value.IsString()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect String but get .");
    }
    t = value.GetString();
}

inline void FromRapidValue(bool& t, RapidValue& value) {
    if (!value.IsBool()) {
        // todo: fill more info to exception
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect bool but get .");
    }
    t = value.GetBool();
}

template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_signed<T>::value, void>
FromRapidValue(T& t, RapidValue& value)
{
    if (!value.IsInt64()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect int64 but get .");
    }
    t = (T)value.GetInt64();
}

template <typename T>
inline enable_if_t<std::is_integral<T>::value && std::is_unsigned<T>::value, void>
FromRapidValue(T& t, RapidValue& value)
{
    if (!value.IsUint64()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect uint64 but get .");
    }
    t = (T)value.GetUint64();
}

template <typename T>
inline enable_if_t<std::is_enum<T>::value, void>
FromRapidValue(T& t, RapidValue& value)
{
    if (!value.IsInt()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect int but get .");
    }
    t = (T)value.GetInt();
}

template <typename T>
inline enable_if_t<std::is_floating_point<T>::value, void>
FromRapidValue(T& t, RapidValue& value)
{
    if (!value.IsDouble()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect double but get .");
    }
    t = (T)value.GetDouble();
}

#define DESERIALIZE_FROM_ARRAY(container, func, value)                  \
    {                                                                   \
        if (!value.IsArray()) {                                         \
            AUTIL_LEGACY_THROW(TypeNotMatchException,                   \
                    "type not match, expect Array but get .");          \
        }                                                               \
        for (auto itr = value.Begin(); itr != value.End(); ++itr) {     \
            T tmpValue;                                                 \
            FromRapidValue(tmpValue, *itr);                             \
            container.func(tmpValue);                                   \
        }                                                               \
    }

template <typename T>
inline void FromRapidValue(std::vector<T>& t, RapidValue& value) {
    DESERIALIZE_FROM_ARRAY(t, push_back, value)
}

template <typename T>
inline void FromRapidValue(std::set<T>& t, RapidValue& value) {
    DESERIALIZE_FROM_ARRAY(t, insert, value)
}

template <typename T>
inline void FromRapidValue(std::deque<T>& t, RapidValue& value) {
    DESERIALIZE_FROM_ARRAY(t, push_back, value)
}

template <typename T>
inline void FromRapidValue(std::list<T>& t, RapidValue& value) {
    DESERIALIZE_FROM_ARRAY(t, push_back, value)
}

template <typename T, typename C>
inline void FromRapidValue(std::map<std::string, T, C>& t, RapidValue& value) {
    if (!value.IsObject()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect Object but get .");
    }
    for (auto itr = value.MemberBegin(); itr != value.MemberEnd(); ++itr) {
        std::string k = itr->name.GetString();
        T v;
        FromRapidValue(v, itr->value);
        t.insert(std::make_pair(k, v));
    }
}

template <typename T, typename U, typename C>
inline void FromRapidValue(std::map<T, U, C>& t, RapidValue& value) {
    if (!value.IsArray()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect Array but get .");
    }
    for (auto itr = value.Begin(); itr != value.End(); ++itr) {
        std::pair<T, U> tmpPair;
        FromRapidValue(tmpPair, *itr);
        t.insert(tmpPair);
    }
}

template <typename T, typename U, typename C>
inline void FromRapidValue(std::multimap<T, U, C>& t, RapidValue& value) {
    if (!value.IsArray()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect Array but get .");
    }
    for (auto itr = value.Begin(); itr != value.End(); ++itr) {
        std::pair<T, U> tmpPair;
        FromRapidValue(tmpPair, *itr);
        t.insert(tmpPair);
    }
}

template <typename T, typename U>
inline void FromRapidValue(std::pair<T, U>& t, RapidValue& value) {
    if (!value.IsArray()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect Array but get .");
    }
    if (value.Size() != 2) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect 2 element Array but get .");
    }
    FromRapidValue(t.first, value[0]);
    FromRapidValue(t.second, value[1]);
}

template <typename T>
inline void FromRapidValue(T*& t, RapidValue& value) {
    if (value.IsNull()) {
        t = NULL;
        return;
    }
#if __cplusplus >= 201103L
    std::unique_ptr<T> p(new T);
#else
    std::auto_ptr<T> p(new T);
#endif
    FromRapidValue(*p, value);
    t = p.release();
}

template <typename T>
inline void FromRapidValue(std::tr1::shared_ptr<T>& t, RapidValue& value) {
    T *p = NULL;
    FromRapidValue(p, value);
    t.reset(p);
}

template <typename T>
inline void FromRapidValue(std::shared_ptr<T>& t, RapidValue& value) {
    T *p = NULL;
    FromRapidValue(p, value);
    t.reset(p);
}

inline void FromRapidValue(ExceptionBase& t, RapidValue& value) {
    if (!value.IsObject()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect Object but get .");
    }
    FromRapidValue(t.mMessage, value["Message"]);
    FromRapidValue(t.mLine, value["Line"]);
    if (value.HasMember("Cause")) {
        FromRapidValue(t.mNestedException, value["Cause"]);
    }
}

inline void FromRapidValue(uint128_t& t, RapidValue& value) {
    if (!value.IsArray()) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect Array but get .");
    }
    if (value.Size() != 2) {
        AUTIL_LEGACY_THROW(TypeNotMatchException,
                           "type not match, expect 2 element Array but get .");
    }
    FromRapidValue(t.value[0], value[0]);
    FromRapidValue(t.value[1], value[1]);
}

// FromRapidValue implemention end

template <typename T>
std::string FastToJsonString(const T& t) {
    autil_rapidjson::StringBuffer s;
    RapidWriter writer(s);
    serializeToWriter(&writer, t);
    return s.GetString();
}

template <typename T>
const char* FastToJsonString(const T& t, autil_rapidjson::StringBuffer &s) {
    RapidWriter writer(s);
    serializeToWriter(&writer, t);
    return s.GetString();
}

template<typename T>
std::string FastToJsonString(const T& t, bool isCompact,
                         const std::string &prefix = "")
{
    // temp for perf test
    return FastToJsonString(t);
}

template <typename T>
void FastFromJsonString(T& t, const std::string& str) {
    RapidDocument document;
    autil_rapidjson::ParseResult ok = document.Parse(str.c_str());
    if (!ok) {
        std::ostringstream errStream;
        errStream << "JSON parse error:" << autil_rapidjson::GetParseError_En(ok.Code())
                  << " offset:" << ok.Offset();
        AUTIL_LEGACY_THROW(WrongFormatJsonException,
                           errStream.str());
    }
    FromRapidValue(t, document);
}


}
}

#endif//AUTIL_LEGACY_JSONIZABLE_H
