#ifndef __BEEPER_COMMON_DEFINE_H
#define __BEEPER_COMMON_DEFINE_H

#include <tr1/memory>
#include "beeper/common/log.h"

#define BP_NAMESPACE_BEGIN(x) namespace beeper { namespace x {
#define BP_NAMESPACE_END(x) }}
#define BP_NAMESPACE_USE(x) using namespace beeper::x
#define BP_NAMESPACE(x) beeper::x


#define BP_DECLARE_REFERENCE_CLASS(x, y)        \
    BP_NAMESPACE_BEGIN(x)                       \
    class y;                                    \
    typedef std::tr1::shared_ptr<y> y##Ptr;     \
    BP_NAMESPACE_END(x)


#define BUILTIN_LOG_BEEPER_RECORDER_TYPE_NAME "log"

#define BEEPER_RECORDER_LIBSO_PATH_ENV_NAME "LD_LIBRARY_PATH"
#define BEEPER_RECORDER_LIBSO_NAME_PREFIX "libbeeper_plugin_"
#define BEEPER_RECORDER_LIBSO_NAME_PATTERN "libbeeper_plugin_[xxx].so.[1]"
#define BEEPER_RECORDER_LIBSO_MAX_VERSION 0xFFFFFFFF

// plugin should implement createBeeperRecorder by new child EventRecorder object
#define BEEPER_RECORDER_DLL_CREATE_FUNCTION_NAME "CreateBeeperRecorder"
#define BEEPER_RECORDER_DLL_DESTROY_FUNCTION_NAME "DestroyBeeperRecorder"

#endif //__BEEPER_COMMON_DEFINE_H
