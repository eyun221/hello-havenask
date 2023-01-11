#ifndef AUTIL_LEGACY_JSONIZABLE_EXCEPTION_H
#define AUTIL_LEGACY_JSONIZABLE_EXCEPTION_H

#include "autil/legacy/exception.h"

namespace autil {
namespace legacy {

class NotJsonizableException : public ExceptionBase
{
public:
    AUTIL_LEGACY_DEFINE_EXCEPTION(NotJsonizableException, ExceptionBase);
};

class TypeNotMatchException : public ExceptionBase
{
public:
    AUTIL_LEGACY_DEFINE_EXCEPTION(TypeNotMatchException, ExceptionBase);
};

class WrongFormatJsonException : public ExceptionBase
{
public:
    AUTIL_LEGACY_DEFINE_EXCEPTION(WrongFormatJsonException, ExceptionBase);
};

class NotSupportException : public ExceptionBase
{
public:
    AUTIL_LEGACY_DEFINE_EXCEPTION(NotSupportException, ExceptionBase);
};

}
}
#endif//AUTIL_LEGACY_JSONIZABLE_EXCEPTION_H
