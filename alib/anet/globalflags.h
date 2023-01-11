#ifndef GLOBAL_FLAGS_H
#define GLOBAL_FLAGS_H

#include <sstream>
#include <cstdint>

namespace anet {
namespace flags {

#define DECLARE_GLOBAL_FLAG(type, name) \
    type get##name(void); \
    void set##name(type newval);

DECLARE_GLOBAL_FLAG(int, ConnectTimeout);
DECLARE_GLOBAL_FLAG(bool, ChecksumState);
DECLARE_GLOBAL_FLAG(int64_t, MaxConnectionCount);
DECLARE_GLOBAL_FLAG(bool, EnableSocketNonBlockConnect);

/* function to dump all configuration. */
int dump(std::ostringstream &buf);

}
}
#endif
