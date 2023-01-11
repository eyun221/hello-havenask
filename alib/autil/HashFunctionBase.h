#ifndef AUTIL_HASHFUNCTIONBASE_H
#define AUTIL_HASHFUNCTIONBASE_H

#include <string>
#include <assert.h>
#include <tr1/memory>
#include "autil/CommonMacros.h"
#include "autil/Log.h"

AUTIL_BEGIN_NAMESPACE(autil);

class HashFunctionBase
{
public:
    HashFunctionBase(const std::string& hashFunction,
                     uint32_t hashSize = 65536)
        : _hashFunction(hashFunction), _hashSize(hashSize)
    {}
    virtual ~HashFunctionBase() {}
public:
    virtual bool init() {
        return true;
    }
    virtual uint32_t getHashId(const std::string& str) const = 0;
    virtual uint32_t getHashId(const char *buf, size_t len) const {
        assert(buf != NULL);
        return getHashId(std::string(buf, len));
    }
public:
    virtual uint32_t getHashId(const std::vector<std::string>& strVec) const;
    virtual std::vector<std::pair<uint32_t, uint32_t> > getHashRange(
            const std::vector<std::string>& strVec) const;
    virtual std::vector<std::pair<uint32_t, uint32_t> > getHashRange(uint32_t partId) const;
    virtual std::vector<std::pair<uint32_t, uint32_t> > getHashRange(uint32_t partId,
            float ratio) const;

    std::string getFunctionName() const;
    uint32_t getHashSize() const;
    const std::set<std::string>& getCanEmptyFields() const {
        return _canEmptyFields;
    }
protected:
    std::string _hashFunction;
    uint32_t _hashSize;
    std::set<std::string> _canEmptyFields;
};
typedef std::tr1::shared_ptr<HashFunctionBase> HashFunctionBasePtr;

const std::string HASH_FUNC_DOCID = "DOCID"; 
const std::string HASH_FUNC_HASH  = "HASH";

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_HASHFUNCTIONBASE_H
