#ifndef FSLIB_SAFEBUFFER_H
#define FSLIB_SAFEBUFFER_H

#include <autil/Log.h>
#include "fslib/common/common_define.h"
#include "fslib/common/common_type.h"

FSLIB_BEGIN_NAMESPACE(util);

class SafeBuffer
{
public:
    SafeBuffer(int64_t size)
        : _size(size)
    {
        _buffer = new char[size];
    }
    
    ~SafeBuffer() {
        if (_buffer) {
            delete[] _buffer;
            _buffer = NULL;
        }
    }

public:
    char* getBuffer() const {
        return _buffer;
    }

    int64_t getSize() const {
        return _size;
    }

private:
    char* _buffer;
    int64_t _size;
};

FSLIB_TYPEDEF_AUTO_PTR(SafeBuffer);

FSLIB_END_NAMESPACE(util);

#endif //FSLIB_SAFEBUFFER_H
