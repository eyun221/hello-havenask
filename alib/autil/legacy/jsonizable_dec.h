#ifndef AUTIL_LEGACY_JSONIZABLE_DECLARE_H
#define AUTIL_LEGACY_JSONIZABLE_DECLARE_H

#if defined(__SSE4_2__)
#  ifndef AUTIL_RAPIDJSON_SSE42
#    define AUTIL_RAPIDJSON_SSE42
#  endif
#elif defined(__SSE2__)
#  ifndef AUTIL_RAPIDJSON_SSE2
#    define AUTIL_RAPIDJSON_SSE2
#  endif
#endif

#include "autil/legacy/json.h"
#include "autil/legacy/jsonizable_exception.h"
#include "autil/legacy/rapidjson/document.h"
#include "autil/legacy/rapidjson/writer.h"

namespace autil {
namespace legacy {

#define FAST_MODE_MAGIC_NUMBER 31415926

typedef autil_rapidjson::GenericValue<autil_rapidjson::UTF8<>> RapidValue;
typedef autil_rapidjson::GenericDocument<autil_rapidjson::UTF8<>> RapidDocument;
typedef autil_rapidjson::Writer<autil_rapidjson::StringBuffer> RapidWriter;
class Jsonizable
{
public:
    enum Mode { TO_JSON, FROM_JSON };

    class JsonWrapper
    {
    public:
        JsonWrapper()
            : mMode(TO_JSON)
            , mFastMagicNum(0)
        {}

        JsonWrapper(const Any& json)
            : mMode(FROM_JSON)
            , mFastMagicNum(0)
        {
            mJsonMap = AnyCast<std::map<std::string, Any> >(json);
        }

        JsonWrapper(RapidWriter *writer)
            : mMode(TO_JSON)
            , mFastMagicNum(FAST_MODE_MAGIC_NUMBER)
            , mWriter(writer)
        {}

        JsonWrapper(RapidValue *value)
            : mMode(FROM_JSON)
            , mFastMagicNum(FAST_MODE_MAGIC_NUMBER)
            , mValue(value)
        {}

        Mode GetMode() const
        { return mMode; }

        int32_t GetFastMagicNum() const
        { return mFastMagicNum; }

        RapidValue* GetRapidValue() const
        { return mValue; }

        std::map<std::string, Any> GetMap()
        { return mJsonMap; }

        template<typename T>
        void Jsonize(const std::string& key, T& value) {
            if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
                FastJsonize(key, value);
            } else {
                AnyJsonize(key, value);
            }
        }

        template<typename T>
        void Jsonize(const std::string& key, T& value, const T& defaultValue) {
            if (mFastMagicNum == FAST_MODE_MAGIC_NUMBER) {
                FastJsonize(key, value, defaultValue);
            } else {
                AnyJsonize(key, value, defaultValue);
            }
        }

        // void JsonizeBinary(const std::string& key, std::string& value) {
        //     if (mFastMagicNum) {
        //         AUTIL_LEGACY_THROW(NotJsonizableException,
        //                 "JsonizeBinary not work in fast mode .");
        //     } else {
        //         AnyJsonizeBinary(key, value);
        //     }
        // }

        // void JsonizeBinary(const std::string& key, std::string& value,
        //                    const std::string& defaultValue)
        // {
        //     if (mFastMagicNum) {
        //         AUTIL_LEGACY_THROW(NotJsonizableException,
        //                 "JsonizeBinary not work in fast mode .");
        //     } else {
        //         AnyJsonizeBinary(key, value, defaultValue);
        //     }
        // }

        void Jsonize(const std::string& key, std::string& value, const std::string& defaultValue)
        { return Jsonize<std::string>(key, value, defaultValue); }

        void Jsonize(const std::string& key, int64_t& value, const int64_t& defaultValue)
        { return Jsonize<int64_t>(key, value, defaultValue); }

    private:
        template<typename T>
        void AnyJsonize(const std::string& key, T& value);

        template<typename T>
        void AnyJsonize(const std::string& key, T& value, const T& defaultValue);

        // void AnyJsonizeBinary(const std::string& key, std::string& value);

        // void AnyJsonizeBinary(const std::string& key, std::string& value, const std::string& defaultValue);

        template <typename T>
        void FastJsonize(const std::string& key, T& value);

        template<typename T>
        void FastJsonize(const std::string& key, T& value, const T& defaultValue);

    public:
        RapidValue *getRapidValue() {
            return mValue;
        }

    private:
        Mode mMode;
        std::map<std::string, Any> mJsonMap;
        // avoid wrong call, assume a lib is compiled by old autil, that means
        // it's jsonwrapper do not have mFastMagicNum field, the access will get
        // an undefined value;
        int32_t mFastMagicNum;
        RapidWriter *mWriter;
        RapidValue *mValue;
    };

    virtual ~Jsonizable() {}
    virtual void Jsonize(JsonWrapper& json) =0;
};


}
}
#endif//AUTIL_LEGACY_JSONIZABLE_declare_H
