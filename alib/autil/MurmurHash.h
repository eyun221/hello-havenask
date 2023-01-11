/*
  Murmurhash from http://sites.google.com/site/murmurhash/

  All code is released to the public domain. For business purposes, Murmurhash is
  under the MIT license.
*/

#ifndef AUTIL_MURMURHASH_H
#define AUTIL_MURMURHASH_H

#include "autil/CommonMacros.h"

AUTIL_BEGIN_NAMESPACE(autil);

class MurmurHash
{
public:
    static uint64_t MurmurHash64A(const void * key, int len, uint64_t seed);
    static uint64_t MurmurHash64A(const void * key, int len, uint64_t seed, uint64_t salt); 
private:
    MurmurHash();
    ~MurmurHash();
    MurmurHash(const MurmurHash &);
    MurmurHash& operator = (const MurmurHash &);
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_MURMURHASH_H
