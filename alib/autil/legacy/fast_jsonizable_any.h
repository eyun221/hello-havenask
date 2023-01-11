
inline void serializeToWriter(RapidWriter *writer,
                              const Any& a) {
        decltype(a.GetType()) &type = a.GetType();
        if (type == typeid(void))                               /// void (null)
        {
            writer->String("null");
        }
        else if (type == typeid(std::vector<Any>))              /// list
        {
            const json::JsonArray& v = *(AnyCast<json::JsonArray>(&a));
            serializeToWriter(writer, v);
        }
        else if (type == typeid(std::map<std::string, Any>))    /// map
        {
            const json::JsonMap& m = *(AnyCast<json::JsonMap>(&a));
            serializeToWriter(writer, m);
        }
        else if (type == typeid(std::string))               /// string
        {
            serializeToWriter(writer, (*AnyCast<std::string>(&a)));
        } else if (type == typeid(bool))                    /// bool
        {
            serializeToWriter(writer, AnyCast<bool>(a));
        }
        /// different numberic types
#define WRITE_DATA(tp)                                             \
        else if (type == typeid(tp))                               \
        {                                                          \
            serializeToWriter(writer, (autil::legacy::AnyCast<tp>(a)));   \
        }
        WRITE_DATA(double)
        WRITE_DATA(float)
        WRITE_DATA(uint16_t)
        WRITE_DATA(int16_t)
        WRITE_DATA(uint32_t)
        WRITE_DATA(int32_t)
        WRITE_DATA(uint64_t)
        WRITE_DATA(int64_t)
#undef WRITE_DATA
#define WRITE_DATA_AS(tp, as)                                                         \
        else if (type == typeid(tp))                                                  \
        {                                                                             \
            serializeToWriter(writer, (static_cast<as>(autil::legacy::AnyCast<tp>(a))));     \
        }
        WRITE_DATA_AS(uint8_t, uint32_t)
        WRITE_DATA_AS(int8_t, int32_t)
#undef WRITE_DATA_AS
        /// end of different numberic types
        else if (type == typeid(json::JsonNumber))         /// number literal
        {
            serializeToWriter(writer, (AnyCast<json::JsonNumber>(a).AsString()));
        }
        else                                            /// other un-supported type
        {
            AUTIL_LEGACY_THROW(
                autil::legacy::ParameterInvalidException, 
                std::string("See unsupported data in Any:") + type.name()
            );
        }
    }

