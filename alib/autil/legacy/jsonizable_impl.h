#ifndef AUTIL_LEGACY_JSONIZABLE_IMPL_H
#define AUTIL_LEGACY_JSONIZABLE_IMPL_H

namespace autil {
namespace legacy {

template <typename T>
void Jsonizable::JsonWrapper::FastJsonize(const std::string& key, T& value) {
    if (mMode == TO_JSON) {
        mWriter->Key(key.c_str(), key.size());
        serializeToWriter(mWriter, value);
    } else {
        if (mValue->HasMember(key.c_str())) {
            FromRapidValue(value, (*mValue)[key]);
        } else {
            AUTIL_LEGACY_THROW(NotJsonizableException, key +
                    " not found when try to parse from Json.");
        }
    }
}

template <typename T>
void Jsonizable::JsonWrapper::FastJsonize(
        const std::string& key, T& value,
        const T& defaultValue)
{
    if (mMode == TO_JSON) {
        mWriter->Key(key.c_str(), key.size());
        serializeToWriter(mWriter, value);
    } else {
        if (mValue->HasMember(key.c_str())) {
            FromRapidValue(value, (*mValue)[key]);
        } else {
            value = defaultValue;
        }
    }
}

template<typename T>
void Jsonizable::JsonWrapper::AnyJsonize(const std::string& key, T& value)
{
    if (mMode == TO_JSON)
        mJsonMap[key] = ToJson(value);
    else
    {
        std::map<std::string, Any>::const_iterator it = mJsonMap.find(key);
        if (it == mJsonMap.end())
        {
            std::stringstream ss;
            ss << "context:{";
            for (std::map<std::string, Any>::const_iterator it = mJsonMap.begin();
                 it != mJsonMap.end();
                 it++)
            {
                ss << "," << it->first;
            }
            ss << "}";
            AUTIL_LEGACY_THROW(NotJsonizableException, key +
                    " not found when try to parse from Json." + ss.str());
        }
        FromJson(value, it->second);
    }
}

template<typename T>
void Jsonizable::JsonWrapper::AnyJsonize(
        const std::string& key, T& value, const T& defaultValue)
{
    if (mMode == TO_JSON)
        mJsonMap[key] = ToJson(value);
    else
    {
        std::map<std::string, Any>::const_iterator it = mJsonMap.find(key);
        if (it == mJsonMap.end())
            value = defaultValue;
        else
            FromJson(value, it->second);
    }
}

// void Jsonizable::JsonWrapper::AnyJsonizeBinary(const std::string& key, std::string& value)
// {
//     if (mMode == TO_JSON)
//     {
//         std::istringstream in(value);
//         std::ostringstream out;
//         Base64Encoding(in, out);
//         mJsonMap[key] = out.str();
//     }
//     else
//     {
//         std::map<std::string, Any>::const_iterator it = mJsonMap.find(key);
//         if (it == mJsonMap.end())
//         {
//             std::stringstream ss;
//             ss << "context:{";
//             for (std::map<std::string, Any>::const_iterator it = mJsonMap.begin();
//                  it != mJsonMap.end();
//                  it++)
//             {
//                 ss << "," << it->first;
//             }
//             ss << "}";
//             AUTIL_LEGACY_THROW(autil::legacy::NotJsonizableException,
//                     key + " not found when try to parse from Json." + ss.str());
//         }
//         std::string v;
//         FromJson(v, it->second);
//         std::istringstream in(v);
//         std::ostringstream out;
//         Base64Decoding(in, out);
//         value = out.str();
//     }
// }

// void Jsonizable::JsonWrapper::AnyJsonizeBinary(
//         const std::string& key, std::string& value,
//         const std::string& defaultValue)
// {
//     if (mMode == TO_JSON)
//     {
//         std::istringstream in(value);
//         std::ostringstream out;
//         Base64Encoding(in, out);
//         mJsonMap[key] = out.str();
//     }
//     else
//     {
//         std::map<std::string, Any>::const_iterator it = mJsonMap.find(key);
//         if (it == mJsonMap.end())
//         {
//             value = defaultValue;
//         }
//         std::string v;
//         FromJson(v, it->second);
//         std::istringstream in(v);
//         std::ostringstream out;
//         Base64Decoding(in, out);
//         value = out.str();
//     }
// }

}
}
#endif//AUTIL_LEGACY_JSONIZABLE_H
