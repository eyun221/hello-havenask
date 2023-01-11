#ifndef AUTIL_CIRCULARQUEUE_H
#define AUTIL_CIRCULARQUEUE_H

#include "autil/CommonMacros.h"
#include <assert.h>
#include <stdint.h>
#include <cstddef>

AUTIL_BEGIN_NAMESPACE(autil);

template<typename T>
class CircularQueue
{
public:
    CircularQueue(size_t capacity = 4 * 1024)
    {
        assert(capacity < ((1L << 31) - 1));
        _items = new T[capacity];
        _capacity = capacity;
        _size = 0;
        _front = 0; 
        _back = 0;
    }
    ~CircularQueue() {delete []_items;}
private:
    CircularQueue(const CircularQueue &);
    CircularQueue& operator = (const CircularQueue &);
public:
    inline void push_front(const T &t) {
        assert(_size < _capacity);
        if (_size == 0) {
            _front = 0;
            _back = 0;
        } else {
            if (--_front == -1) {
                _front = _capacity - 1;
            }
        }
        ++_size;
        _items[_front] = t;
    }
    
    inline void push_back(const T &t) {
        assert(_size < _capacity);
        if (_size == 0) {
            _front = 0;
            _back = 0;
        } else {
            if (++_back == _capacity) {
                _back = 0;
            }
        }
        ++_size;
        _items[_back] = t;
    }
    
    inline void pop_front() {
        assert(_size);
        if (++_front == _capacity) {
            _front = 0;
        }
        --_size;
    }
    
    inline void pop_back() {
        assert(_size);
        if (--_back == -1) {
            _back = _capacity - 1;
        }
        --_size;
    }
    
    inline T& front() {return _items[_front];} 
    inline T& back() {return _items[_back];}
    
    inline const T& front() const {return _items[_front];} 
    inline const T& back() const {return _items[_back];} 
    
    inline size_t size() const {return _size;}
    inline size_t capacity() const {return _capacity;}
    inline bool empty() const {return _size == 0;}
private:
    T *_items;
    int32_t _capacity;
    int32_t _size;
    int32_t _front;
    int32_t _back;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_CIRCULARQUEUE_H
