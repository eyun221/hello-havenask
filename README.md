This is a demo project use to build havenask (a search engine from alibaba https://github.com/alibaba/havenask) with bazel

Some dependencies are copied from docker havenask/ha3_runtime:0.2.2, cpp header files from /ha3_depends/usr/local/include,  and static library .a files from /ha3_depends/usr/local/lib64

maybe you need to run some commands to install open-source dependencies

### yum install libaio-devel snappy libzstd-devel lz4-devel zlib-devel

add LZ4HC_DEFAULT_CLEVEL macro to /usr/include/lz4hc.h for your env (https://github.com/lz4/lz4/blob/v1.7.3/lib/lz4hc.h),
or use LZ4HC_CLEVEL_DEFAULT for havenask/aios/indexlib/indexlib/util/buffer_compressor/lz4_hc_compressor.h

### #define LZ4HC_DEFAULT_CLEVEL    9


Only indexlib is tested at the current time.
Enjoy it!
