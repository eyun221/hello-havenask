#ifndef AUTIL_POOLMAP_H
#define AUTIL_POOLMAP_H

#include "autil/CommonMacros.h"
#include "autil/mem_pool/Pool.h"
#include "autil/mem_pool/PoolVector.h"
#include <algorithm>

AUTIL_BEGIN_NAMESPACE_2(autil, mem_pool)

template <typename KeyType, typename ValueType>
class PoolMap
{
public:
    static const size_t NEED_SORTED_THRESHOLD = 16;
    typedef PoolVector<std::pair<KeyType, ValueType> >Vector;
    typedef typename Vector::iterator iterator;
    typedef typename Vector::const_iterator const_iterator;
public:
    PoolMap(Pool* pool) {
        _pairs = new (pool->allocate(sizeof(Vector)))Vector(pool);
        _pool = pool;
        _isSorted = false;
    };
    ~PoolMap() {
        _pairs->~Vector();
        _pool->deallocate(_pairs, sizeof(Vector));
    };
private:
    PoolMap(const PoolMap &) {};
    PoolMap& operator=(const PoolMap &) {};
public:
    void sort() const {
        if (_isSorted) {
            return;
        }
        std::sort(_pairs->begin(), _pairs->end());
        _isSorted = true;
    }
public:
    size_t size() const {return _pairs->size();}
    bool empty() const {return _pairs->empty();}

    iterator begin() {return _pairs->begin();}
    iterator end() {return _pairs->end();}
    const_iterator begin() const {return _pairs->begin();}
    const_iterator end() const {return _pairs->end();}

    ValueType& operator[] (const KeyType& key) {
        iterator it = sequentialSearch<iterator>(key);
        if (_pairs->end() != it) {
            return it->second;
        }
        _pairs->push_back(make_pair(key, ValueType()));
        _isSorted = false;
        return _pairs->back().second;
    }
    iterator find(const KeyType& key) {
        return doFind<iterator>(key);
    }
    const_iterator find(const KeyType& key) const {
        return doFind<const_iterator>(key);
    }
private:
    template<typename iterator_type>
    iterator_type doFind(const KeyType& key) const {
        if (false == _isSorted && (_pairs->size() < NEED_SORTED_THRESHOLD)) {
            return sequentialSearch<iterator_type>(key);
        }
        sort();
        return binarySearch<iterator_type>(key);
    }

    template<typename iterator_type>
    iterator_type sequentialSearch(const KeyType& key) const {
        for (iterator_type it = _pairs->begin(); it != _pairs->end(); ++it) {
            if (key == it->first) {
                return it;
            }
        }
        return _pairs->end();
    }

    template<typename iterator_type>
    iterator_type binarySearch(const KeyType& key) const {
        size_t low = 0;
        size_t high = _pairs->size() - 1;
        while(low < high) {
            size_t mid = (low + high) >> 1;
            if ((*_pairs)[mid].first < key) {
                low = mid + 1;
            } else {
                high = mid;
            }
        }

        if ((low == high) && ((*_pairs)[low].first) == key) {
            return _pairs->begin() + low;
        }
        return _pairs->end();
    }
private:
    Pool* _pool;
    mutable bool _isSorted;
    mutable Vector *_pairs;
private:
    friend class PoolMapTest;
};

AUTIL_END_NAMESPACE_2(autil, mem_pool)
#endif //AUTIL_POOLMAP_H
