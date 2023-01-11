#ifndef FSLIB_MOCKFS_H
#define FSLIB_MOCKFS_H

#include <unordered_map>
#include <autil/Log.h>
#include "fslib/fslib.h"
#include "fslib/fs/mock/common.h"

FSLIB_PLUGIN_BEGIN_NAMESPACE(mock);
class MockFileSystem;

class MockFS
{
public:
    typedef std::function<ErrorCode(const std::string &path, const std::string &args, std::string &output)> ForwardFunc;

public:
    MockFS() = delete;
    ~MockFS() = delete;

public:
    static void mockForward(const std::string& command, const ForwardFunc& func);
    static void setLocalRoot(const std::string& root);
    static std::string getLocalRoot();

private:
    static MockFileSystem* getMockFs();
};

FSLIB_TYPEDEF_AUTO_PTR(MockFS);

FSLIB_PLUGIN_END_NAMESPACE(mock);

#endif //FSLIB_MOCKFS_H
