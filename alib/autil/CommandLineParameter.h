#ifndef AUTIL_COMMANDLINEPARAMETER_H
#define AUTIL_COMMANDLINEPARAMETER_H

#include "autil/CommonMacros.h"
#include <string>

AUTIL_BEGIN_NAMESPACE(autil);

class CommandLineParameter
{
public:
    CommandLineParameter(const std::string& cmd);
    ~CommandLineParameter();
public:
    inline int getArgc() const { return _argc; };
    inline char** getArgv() const { return _argv; };
private:
    int _argc;
    char** _argv;
};

AUTIL_END_NAMESPACE(autil);

#endif //AUTIL_COMMANDLINEPARAMETER_H
