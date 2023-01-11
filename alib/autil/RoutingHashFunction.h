#ifndef AUTIL_ROUTINGHASHFUNCTION_H
#define AUTIL_ROUTINGHASHFUNCTION_H

#include "autil/CommonMacros.h"
#include "autil/HashFunctionBase.h"
#include <unordered_map>
#include <math.h>

AUTIL_BEGIN_NAMESPACE(autil);
class RoutingHashFunction : public HashFunctionBase
{
public:
    RoutingHashFunction(const std::string& hashFunction,
                        const std::map<std::string, std::string> &params,
                        uint32_t partitionCount)
        : HashFunctionBase(hashFunction, partitionCount)
        , _params(params)
        , _routingRatio(0)
    {}
    virtual ~RoutingHashFunction() {}
public:
    bool init() override;
    uint32_t getHashId(const std::string& str) const override;
    uint32_t getHashId(const char *buf, size_t len) const override;
    uint32_t getHashId(const std::vector<std::string>& strVec) const override;
    std::vector<std::pair<uint32_t, uint32_t> > getHashRange(
            const std::vector<std::string>& strVec) const override;
    std::vector<std::pair<uint32_t, uint32_t> > getHashRange(
            uint32_t partId) const override;
    std::vector<std::pair<uint32_t, uint32_t> > getHashRange(
            uint32_t partId, float ratio) const override;

    float getRoutingRatio() { return _routingRatio; }

private:
    uint32_t getHashId(const std::string& str1, const std::string& str2) const;
    uint32_t getHashId(const std::string& str1, const std::string& str2, float ratio) const;
    std::vector<std::pair<uint32_t, uint32_t> > getInnerHashRange(
            const std::string& str) const;
    std::vector<std::pair<uint32_t, uint32_t> > getInnerHashRange(
            const std::string& str, float ratio) const;
    std::vector<std::pair<uint32_t, uint32_t> > getInnerHashRange(
            uint32_t hashVal, float ratio) const;
    bool parseHotValues(const std::string& key, const std::string& value);
    bool parseHotRanges(const std::string& key, const std::string& value);
    bool initFuncBase(const std::string& funcName);
    uint32_t getLogicId(const std::string& value) const;
    uint32_t getLogicId(const char *buf, size_t len) const;

private:
    std::map<std::string, std::string> _params;
    float _routingRatio;
    std::unordered_map<std::string, float> _hotValueMap;
    std::vector<float> _hotRangeVec;
    HashFunctionBasePtr _funcBase;
private:
    AUTIL_LOG_DECLARE();
private:
    friend class RoutingHashFunctionTest;
};

typedef std::tr1::shared_ptr<RoutingHashFunction> RoutingHashFunctionPtr;

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_ROUTINGHASHFUNCTION_H
