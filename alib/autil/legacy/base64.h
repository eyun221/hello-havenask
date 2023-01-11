#ifndef AUTIL_LEGACY_BASE64_H
#define AUTIL_LEGACY_BASE64_H

#include "autil/legacy/exception.h"
#include <iostream>

namespace autil {
namespace legacy {

class BadBase64Exception : public ExceptionBase
{
public:
    AUTIL_LEGACY_DEFINE_EXCEPTION(BadBase64Exception, ExceptionBase);
};

void Base64Encoding(std::istream&, std::ostream&, char makeupChar = '=',
                    const char *alphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/");
void Base64Decoding(std::istream&, std::ostream&, char plus = '+', char slash = '/');
void Base64DecodingEx(std::istream&, std::ostream&, char makeupChar = '=', char plus = '+', char slash = '/');

std::string Base64DecodeFast(const std::string &str);
std::string Base64EncodeFast(const std::string &str);

}
}
#endif
