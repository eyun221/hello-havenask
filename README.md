This is a demo project use to build havenask (a search engine from alibaba https://github.com/alibaba/havenask) with bazel

Some dependencies are copied from docker havenask/ha3_runtime:0.2.2, cpp header files from /ha3_depends/usr/local/include,  and static library .a files from /ha3_depends/usr/local/lib64

maybe you need to run some commands to install open-source dependencies
yum install libaio-devel snappy libzstd-devel lz4-devel zlib-devel 


add this macros to /usr/include/lz4hc.h for your env
#define LZ4HC_MIN_CLEVEL        3
#define LZ4HC_DEFAULT_CLEVEL    9
#define LZ4HC_MAX_CLEVEL        16

Only indexlib is tested at the current time.
 Enjoy it!
